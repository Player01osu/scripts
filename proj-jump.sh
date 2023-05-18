#!/bin/sh

# WAIT LOL THIS IS BASICALLY Z
# not really tho cuz specifically for projects
# and w/ fzf

# Goals:
# * auto-add 'jumps'
# * show project name rather than full path
# * be able to add jumps w/ $proj-jump --add .

# Note:
# Absolute paths must be used in jump 🙄

__parse_path__ () {

	SUB=${1/~/$HOME}
	CHECK=${1:0:1}

	if [ -z $CHECK ]; then
		SUB=" "
		return 1
	fi

	if [ $CHECK != "/" ]; then
		case $CHECK in
			".")
				SUB="$(pwd)"
				;;
			*)
				SUB="$(pwd)/$1"
				;;
		esac
	fi

	/usr/bin/ls $SUB &>/dev/null || SUB="PATHINV"
}

__tac_fzf__ () {
    echo $(fzf --tac < $XDG_DATA_HOME/project/jump)
}

pj () {
	case $1 in
		"--add"|"-a")
			__parse_path__ $2
			CHECK=${SUB:0:1}

			# Errors
			if [ $CHECK != "/" ]; then
				case $SUB in
					"PATHINV")
						echo "pj: Invalid path"
						return 1
						;;
					" ")
						echo "pj: Not enough arguments"
						return 1
						;;
					*)
						echo "pj: Unable to add path"
						return 1
						;;
				esac
			fi
			echo $SUB >> $XDG_DATA_HOME/project/jump
			;;
		"--remove"|"-r")
			# fzf path to remove
            tmp="$(__tac_fzf__)"
			if [ -z $tmp ]; then
				return 0
			fi
			REM=$(grep -v $tmp $XDG_DATA_HOME/project/jump)
			echo $REM | sed 's/ /\n/g' > $XDG_DATA_HOME/project/jump
			;;
        "--bump"|"-b")
            # Bumps path to top of list
            tmp="$(__tac_fzf__)"
			if [ -z $tmp ]; then
				return 0
			fi
            bump=$(printf "$(grep -v $tmp $XDG_DATA_HOME/project/jump)\n${tmp}")
			echo $bump | sed 's/ /\n/g' > $XDG_DATA_HOME/project/jump
            ;;
		"")

			# No args fzf
			tmp="$(__tac_fzf__)"
			cd "$tmp"
			;;
		*)
			echo "pj: Invalid argument"
		esac
}

