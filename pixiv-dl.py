#!/usr/bin/env python3

import subprocess
import requests
import sys
import os
import datetime
import json

program_name = sys.argv[0]

help = f'''Usage: {program_name} [OPTION] [artwork-url]

artwork-url are in the form "https://www.pixiv.net/en/artworks/{{ID}}"

 -P {{output dir}}
 -h Print this message'''

def parse_pixiv_link(link):
    valid_link = 'pixiv.net' in link and 'artworks' in link
    # Strip trailing slash
    if link[-1] == '/': link = link[:-1]
    idx = 0
    last_slash_idx = 0
    while idx < len(link):
        if link[idx] == '/': last_slash_idx = idx
        idx += 1
    return (valid_link, link[last_slash_idx + 1:])

def pixiv_link_mdate_super_hack(link, html):
    success, pixiv_id = parse_pixiv_link(link)
    assert success
    idx = 0
    needle = f'{str(pixiv_id)}_p0'
    found_needle = False
    while idx < len(html):
        if html[idx:][:len(needle)] == needle:
            found_needle = True
            break
        idx += 1
    if not found_needle:
        print(f'{program_name}:Could not find image link in html:"{link}"')
        return (False, None)

    idx -= 1
    start_mdate = idx
    end_mdate = idx
    while idx > 0 and (html[idx - 1].isdigit() or html[idx - 1] == '/'):
        start_mdate = idx
        idx -= 1
    mdate = html[start_mdate:end_mdate]

    return (True, mdate)

def pixiv_link_mdate(link):
    r = requests.get(url = link)
    if r.status_code < 200 or r.status_code > 299:
        print(f'{program_name}:Could not connect to pixiv server:"{link}"')
        return (False, None)
    html = r.text
    idx = 0
    needle = 'mdate='
    found_needle = False
    while idx < len(html):
        if html[idx:][:len(needle)] == needle:
            found_needle = True
            break
        idx += 1

    if not found_needle:
        print(f'{program_name}:Could not find mdate:"{link}"')
        return pixiv_link_mdate_super_hack(link, html)
        return (False, None)
    idx += len(needle)
    end_idx = idx
    while html[end_idx].isdigit(): end_idx += 1

    unix_time_int = int(html[idx:end_idx])
    return (True, datetime.datetime.fromtimestamp(unix_time_int, datetime.timezone(datetime.timedelta(hours=9))).strftime('%Y/%m/%d/%H/%M/%S'))

def pixiv_get_image_links_fallback(link, start=0, end=0):
    success, pixiv_id = parse_pixiv_link(link)
    if not success:
        print(f'{program_name}:Not valid pixiv link:"{link}"')
        return (False, None)

    success, mdate = pixiv_link_mdate(link)
    if not success:
        print(f'{program_name}:Could not get mdate:"{link}"')
        return (False, None)
    image_links = []
    print(f'end:{end}')
    for i in range(start, end):
        image_link=f'https://i.pximg.net/img-original/img/{mdate}/{pixiv_id}_p{i}.png'
        image_link_jpg=f'https://i.pximg.net/img-original/img/{mdate}/{pixiv_id}_p{i}.jpg'
        image_links += [image_link, image_link_jpg]
    print(image_links)
    return (True, image_links)

def pixiv_get_image_links(link):
    success, pixiv_id = parse_pixiv_link(link)
    if not success:
        print(f'{program_name}:Could not get pixiv id from link:"{link}"')
        return (False, None, None)
    r = requests.get(url = link)
    if r.status_code < 200 or r.status_code > 299:
        print(f'{program_name}:Could not connect to pixiv server:"{link}"')
        return (False, None, None)
    html = r.text
    needle = 'content='
    found_needle = False
    idx = 0
    content_idx = 0
    while idx < len(html):
        if html[idx:][:len(needle)] == needle:
            content_idx = idx
            found_needle = True
        idx += 1
    if not found_needle:
        print(f'{program_name}:Could not find "{needle}" within link url:{link}')
        return (False, None, None)
    idx = content_idx + len(needle)
    num_single_quotes = 0
    start_json_idx = 0
    end_json_idx = 0
    while idx < len(html) and num_single_quotes < 2:
        if html[idx] == '\'':
            num_single_quotes += 1
            if num_single_quotes == 1:
                start_json_idx = idx + 1
            elif num_single_quotes == 2:
                end_json_idx = idx
        idx += 1
    if num_single_quotes != 2:
        print(f'{program_name}:Bad data from pixiv link:"{link}"')
        return (False, None, None)
    try:
        content_json = json.loads(html[start_json_idx:end_json_idx])
        artwork_meta = content_json['illust'][pixiv_id]
        urls = artwork_meta['urls']
        if urls['original'] is not None:
            return (True, [urls['original'], urls['regular'], urls['small']], artwork_meta['userIllusts'][pixiv_id])
        else:
            return (True, None, artwork_meta['userIllusts'][pixiv_id])
    except:
        print(f'{program_name}:Could not json from pixiv link:"{link}"')
        return (False, None, None)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'{program_name}:Not enough arguments')
        print(help)
        sys.exit(1)

    if '-h' in sys.argv or '--help' in sys.argv:
        print(help)
        sys.exit(0)

    prefix_dir = ""
    start_num = 0
    end_num = 0
    prefix_dir_flag = False
    start_num_flag = False
    end_num_flag = False

    # XXX: Allow downloading artworks with multiple images
    for arg in sys.argv[1:]:
        if arg == '-P':
            prefix_dir_flag = True
        elif prefix_dir_flag:
            prefix_dir = arg
            prefix_dir_flag = False
        elif arg == '-s':
            start_num_flag = True
        elif start_num_flag:
            start_num = int(arg)
            start_num_flag = False
        elif arg == '-e':
            end_num_flag = True
        elif end_num_flag:
            end_num = int(arg)
            end_num_flag = False
        else:
            link = arg
            page_count = 1
            success, image_links, meta = pixiv_get_image_links(link)
            if not success:
                success, image_links = pixiv_get_image_links_fallback(link)
                if not success:
                    print(f'{program_name}:Could not get image link:"{link}"')
                    continue
            else:
                if image_links is None:
                    if end_num == 0:
                        end_num = meta['pageCount']
                    success, image_links = pixiv_get_image_links_fallback(link, start=start_num, end=end_num)
                    page_count = end_num - start_num
                    start_num = 0
                    end_num = 0
                    if not success:
                        print(f'{program_name}:Could not get image link:"{link}"')
                        continue
                # Write metadata
                f = open(os.getenv('XDG_DATA_HOME') + '/pixiv-meta', 'a')
                f.write(f'{meta}\n')
                f.close()

            download_succeed = False
            for image_link in image_links:
                print(image_link)
                wget_args = ['wget',
                             '--header=User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:129.0) Gecko/20100101 Firefox/129.0',
                             '--header=Accept: image/avif,image/webp,image/png,image/svg+xml,image/*;q=0.8,*/*;q=0.5',
                             '--header=Accept-Language: en-US,en;q=0.5',
                             '--header=Accept-Encoding: gzip, deflate, br, zstd',
                             '--header=DNT: 1',
                             '--header=Connection: keep-alive',
                             '--header=Referer: https://www.pixiv.net/',
                             '--header=Sec-Fetch-Dest: image',
                             '--header=Sec-Fetch-Mode: no-cors',
                             '--header=Sec-Fetch-Site: cross-site',
                             '--header=Priority: u=4, i',
                             '--header=Pragma: no-cache',
                             '--header=Cache-Control: no-cache']

                if len(prefix_dir) > 0:
                    wget_args += ['-P', prefix_dir]
                p = subprocess.run(wget_args + [image_link])
                if p.returncode == 0:
                    page_count -= 1
                    download_succeed = True
                if page_count == 0:
                    break

            if not download_succeed:
                print(f'{program_name}:Could not retrieve image:"{link}"')
                continue
