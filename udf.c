#if 0
set -e
cc -o udf udf.c -pedantic -DPWD="s($(pwd))" -DCONFIG="d(${XDG_CONFIG_HOME})" -DHOME="d(${HOME})" -DBIN="d(${HOME}/.local/bin)"
./udf
rm udf
exit
#endif

#define s(x) #x
#define c(s) PWD "/" s
#define d(x) s(x) "/"
#define O1   (void *) 69
#define O0   (void *) 0

/*********************************************************************************************/
/*********************************************************************************************/

/******************/
/* Configurations */
/******************/

/* Sudo program */
const char *sudo = "/usr/bin/doas";

/* Replace old links if exist */
const int replace_old_link = 1;

/* Set the path and link location of files */
const char *links[][4] = {
        /* Config                            Link location                Sudo     Enabled  */
	{ c("addbookmark"),                 BIN "/addbookmark",            0,       O0 },
	{ c("als"),                         BIN "/als",                    0,       O0 },
	{ c("ani"),                         BIN "/ani",                    0,       O0 },
	{ c("anid"),                        BIN "/anid",                   0,       O0 },
	{ c("anki_audio"),                  BIN "/anki_audio",             0,       O0 },
	{ c("autoendsh"),                   BIN "/autoendsh",              0,       O0 },
	{ c("autostartsh"),                 BIN "/autostartsh",            0,       O0 },
	{ c("bar"),                         BIN "/bar",                    0,       O0 },
	{ c("bookmarks"),                   BIN "/bookmarks",              0,       O0 },
	{ c("bookmarksnw"),                 BIN "/bookmarksnw",            0,       O0 },
	{ c("cmark"),                       BIN "/cmark",                  0,       O0 },
	{ c("compositorp"),                 BIN "/compositorp",            0,       O0 },
	{ c("config"),                      BIN "/config",                 0,       O0 },
	{ c("cwp"),                         BIN "/cwp",                    0,       O0 },
	{ c("dani"),                        BIN "/dani",                   0,       O0 },
	{ c("danie"),                       BIN "/danie",                  0,       O0 },
	{ c("escript"),                     BIN "/escript",                0,       O0 },
	{ c("ffmpeg_offset_sub"),           BIN "/ffmpeg_offset_sub",      0,       O0 },
	{ c("ffmpeg_sub"),                  BIN "/ffmpeg_sub",             0,       O0 },
	{ c("invert"),                      BIN "/invert",                 0,       O0 },
	{ c("krepeat"),                     BIN "/krepeat",                0,       O0 },
	{ c("lfub"),                        BIN "/lfub",                   0,       O0 },
	{ c("mid3cp"),                      BIN "/mid3cp",                 0,       O0 },
	{ c("mid3iconv"),                   BIN "/mid3iconv",              0,       O0 },
	{ c("mid3v2"),                      BIN "/mid3v2",                 0,       O0 },
	{ c("mkv_sub"),                     BIN "/mkv_sub",                0,       O0 },
	{ c("moggsplit"),                   BIN "/moggsplit",              0,       O0 },
	{ c("move_anime"),                  BIN "/move_anime",             0,       O0 },
	{ c("move_le_anime"),               BIN "/move_le_anime",          0,       O0 },
	{ c("move_script"),                 BIN "/move_script",            0,       O0 },
	{ c("mpdup"),                       BIN "/mpdup",                  0,       O0 },
	{ c("mutagen-inspect"),             BIN "/mutagen-inspect",        0,       O0 },
	{ c("mutagen-pony"),                BIN "/mutagen-pony",           0,       O0 },
	{ c("nightlight"),                  BIN "/nightlight",             0,       O0 },
	{ c("normalizer"),                  BIN "/normalizer",             0,       O0 },
	{ c("osu"),                         BIN "/osu",                    0,       O0 },
	{ c("pixiv-dl.py"),                 BIN "/pixiv-dl.py",            0,       O0 },
	{ c("pradd"),                       BIN "/pradd",                  0,       O0 },
	{ c("proj-jump.sh"),                BIN "/proj-jump.sh",           0,       O0 },
	{ c("pygmentize"),                  BIN "/pygmentize",             0,       O0 },
	{ c("pytube"),                      BIN "/pytube",                 0,       O0 },
	{ c("rdblk"),                       BIN "/rdblk",                  0,       O0 },
	{ c("removebookmark"),              BIN "/removebookmark",         0,       O0 },
	{ c("rkd"),                         BIN "/rkd",                    0,       O0 },
	{ c("rpd"),                         BIN "/rpd",                    0,       O0 },
	{ c("rpipe"),                       BIN "/rpipe",                  0,       O0 },
	{ c("rptimeshift"),                 BIN "/rptimeshift",            0,       O0 },
	{ c("screencopy"),                  BIN "/screencopy",             0,       O0 },
	{ c("screencopy2"),                 BIN "/screencopy2",            0,       O0 },
	{ c("screenname"),                  BIN "/screenname",             0,       O0 },
	{ c("screensave"),                  BIN "/screensave",             0,       O0 },
	{ c("screenshot-to"),               BIN "/screenshot-to",          0,       O0 },
	{ c("screenshotwindow"),            BIN "/screenshotwindow",       0,       O1 },
	{ c("screenwhole"),                 BIN "/screenwhole",            0,       O0 },
	{ c("screenwholename"),             BIN "/screenwholename",        0,       O0 },
	{ c("screenwholesave"),             BIN "/screenwholesave",        0,       O0 },
	{ c("spotdl"),                      BIN "/spotdl",                 0,       O0 },
	{ c("stream-twitch"),               BIN "/stream-twitch",          0,       O0 },
	{ c("sus"),                         BIN "/sus",                    0,       O0 },
	{ c("sus-backup"),                  BIN "/sus-backup",             0,       O0 },
	{ c("todon"),                       BIN "/todon",                  0,       O0 },
	{ c("toggle_fcitx"),                BIN "/toggle_fcitx",           0,       O0 },
	{ c("toggle_touch"),                BIN "/toggle_touch",           0,       O0 },
	{ c("touchsets"),                   BIN "/touchsets",              0,       O0 },
	{ c("umpv"),                        BIN "/umpv",                   0,       O0 },
	{ c("unidecode"),                   BIN "/unidecode",              0,       O0 },
	{ c("unkeym"),                      BIN "/unkeym",                 0,       O0 },
	{ c("yt-dlp"),                      BIN "/yt-dlp",                 0,       O0 },
	{ c("zpdf"),                        BIN "/zpdf",                   0,       O0 },
};

/*********************************************************************************************/
/*********************************************************************************************/

/**********/
/* Script */
/**********/
#undef d
#undef c
#undef s

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define ARR_LEN(arr) (sizeof(arr) / sizeof(*arr))

const char *program_name;

static int exists(const char *path)
{
	return access(path, F_OK) == 0;
}

static void concat(char *buf, ...)
{
	va_list ap;
	char    *s;

	buf[0] = '\0';
	va_start(ap, buf);
	s = va_arg(ap, char *);
	while (s) {
		strcat(buf, s);
		s = va_arg(ap, char *);
	}
	va_end(ap);
}

static int link_file(const char *from, const char *to)
{
	int  ret;
	struct stat sb;

	fprintf(stdout, "\"%s\" => \"%s\":\n", from, to);

	ret = lstat(to, &sb);
	if (ret == 0) {
		if (sb.st_mode & S_IFLNK) {
			if (replace_old_link) {
				fprintf(stdout, "  Symlink already exists at \"%s\"; replacing link...\n", to);
				if (remove(to) < 0) {
					fprintf(stderr, "%s: Could not remove symlink \"%s\": %s\n", program_name, to, strerror(errno));
					return -1;
				}
			} else {
				fprintf(stdout, "  Symlink already exists at \"%s\"...\n", to);
			}
		}
	}

	ret = symlink(from, to);

	if (ret && errno == EEXIST) {
		fprintf(stdout, "  \"%s\" to \"%s\": File already exists; Continuing...\n", from, to);
	} else if (ret) {
		fprintf(stderr, "%s: Could not symlink file \"%s\" to \"%s\": %s\n", program_name, from, to, strerror(errno));
		return -1;
	} else {
		fprintf(stdout, "  Linking \"%s\" to \"%s\"...\n", from, to);
	}
	return 0;
}

static void pop_path(char *buf, const char *path)
{
	const char *p;
	char       *p_buf;
	p     = path;
	p_buf = buf;
	/* Seek to end */
	while (*p) {
		++p;
	}
	--p;
	/* Remove trailing '/' */
	while (p > path && *p == '/') --p;
	/* Pop until '/' */
	while (p > path && *p != '/') --p;
	/* Remove trailing '/' */
	while (p > path && *p == '/') --p;
	/* Consume path until p, pushing onto buf */
	while (path <= p && *path) {
		*p_buf = *path;
		++p_buf;
		++path;
	}
	*p_buf = '\0';
}

int main(int argc, char **argv)
{
	size_t i;
	int    root_once;
	uid_t  uid;
	uid_t  gid;

	program_name = argv[0];
	root_once    = 0;
	uid          = getuid();
	gid          = getgid();

	if (argc == 3) {
		uid = atoi(argv[1]);
		gid = atoi(argv[2]);
	}

	/* If program already has root, assume this check has already been performed */
	if (getuid()) {
		/* Check files exist and check whether needs sudo */
		for (i = 0; i < ARR_LEN(links); ++i) {
			const char *from;
			const char *to;
			const char *require_root;
			const char *enabled;
			char to_buf[512];

			from         = links[i][0];
			to           = links[i][1];
			require_root = links[i][2];
			enabled      = links[i][3];

			if (!enabled) continue;

			if (!exists(from)) {
				fprintf(stderr, "%s: Could not stat src file: \"%s\"\n", program_name, from);
				return 1;
			}

			pop_path(to_buf, to);
			if (!exists(to_buf)) {
				fprintf(stderr, "%s: Could not stat dest file: \"%s\" for \"%s\"\n", program_name, to_buf, to);
				return 1;
			}
			if (require_root) {
				root_once = 1;
			}
		}

		if (root_once) {
			char uid_buf[32];
			char gid_buf[32];

			sprintf(uid_buf, "%d", uid);
			sprintf(gid_buf, "%d", gid);
			execl(sudo, program_name, program_name, uid_buf, gid_buf, NULL);
			fprintf(stderr, "%s: Could not exec sudo: \"%s\"\n", program_name, sudo);
			fprintf(stderr, "Check if you have the program installed or change `sudo` to appropriate path\n");
			return 1;
		}
	}

	/* Do all sudo links */
	if (!getuid()) {
		for (i = 0; i < ARR_LEN(links); ++i) {
			const char *from;
			const char *to;
			const char *require_root;
			const char *enabled;

			from         = links[i][0];
			to           = links[i][1];
			require_root = links[i][2];
			enabled      = links[i][3];

			if (!enabled) continue;

			if (require_root) {
				if (link_file(from, to) < 0) return 1;
			}
		}
	}

	/* Drop privileges */
	if (!getuid()) {
		if (setgid(gid)) {
			fprintf(stderr, "%s: Could not drop group privileges: %s\n", program_name, strerror(errno));
			return 1;
		}
		if (setuid(uid)) {
			fprintf(stderr, "%s: Could not drop user privileges: %s\n", program_name, strerror(errno));
			return 1;
		}
	}

	/* Do all non-sudo links */
	for (i = 0; i < ARR_LEN(links); ++i) {
		const char *from;
		const char *to;
		const char *require_root;
		const char *enabled;

		from         = links[i][0];
		to           = links[i][1];
		require_root = links[i][2];
		enabled      = links[i][3];

		if (!enabled) continue;

		if (!require_root) {
			if (link_file(from, to) < 0) return 1;
		}
	}
	return 0;
}
