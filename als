#!/bin/sh
# adds local script
if [ -z "$1" ]
then
  prompt () {
  	# prompts the user with message in $1-2 ($1 in blue, $2 in magenta) and saves the input to the variables in $REPLY and $REPLY2
  	printf "\033[1;34m%s\033[1;35m%s\033[1;34m: \033[0m" "$1" "$2"
  	read -r REPLY
  }
  prompt "enter script name"
  SNAME="$REPLY"
else
  SNAME="$1"
fi


bin="$HOME/.local/bin"
#echo $SNAME

touch $bin/$SNAME && chmod +x $bin/$SNAME && echo "#!/bin/sh" >> $bin/$SNAME && nvim $bin/$SNAME
