#!/bin/bash
# echo Parameters passed to merge-tool: $*
# base=${1?1st argument is 'base' file}
# theirs=${2?2nd argument is 'theirs' file}
# mine=${3?3rd argument is 'mine' file}
# merged=${4?4th argument is 'merged' file}
# version=$(meld --version | perl -pe '($_)=/([0-9]+([.][0-9]+)+)/' )    
echo '@' Parameters passed to merge-tool: "$@"
echo '*' Parameters passed to merge-tool: "$*"
LLabel=${5}
RLabel=${3}
LFile=${7}
RFile=${6}
# version=$(meld --version | perl -pe '($_)=/([0-9]+([.][0-9]+)+)/' )    

# echo base is ____ $base 
# echo theirs is __ $theirs
# echo mine is ____ $mine
# echo merged _____ $merged
# echo version ____ $version    
# echo ---------------
# echo "$@"
# echo ---------------

if [[ ! "$SSH_CONNECTION" || -n "$DISPLAY" ]]
then 
	meld --label="$LLabel"    "$LFile"   \
	     --label="$RLabel"    "$RFile"   
else
	TITLE="${LLabel} - ${RLabel}"
	TITLE=$(echo ${TITLE//(/\\(})
	TITLE=$(echo ${TITLE//)/\\)})
	TITLE=$(echo ${TITLE// /\\ })
	TITLE=$(echo ${TITLE//-/\\-})

	if [ "$TMUX" ] 
	then 
	  # TODO in a new window ?
	  # tmux new-window -n  "svndiff"  /usr/bin/vimdiff -c "set title titlestring=$TITLE" ${7} ${6}
	  # <<< this works well for diff working copy, but fails in comparing revisions like r17:14
	  # apparently wiping out tmp files before they get into vimdiff
	  /usr/bin/vimdiff -c "set title titlestring=$TITLE" ${LFile} ${RFile}
	else 
	  /usr/bin/vimdiff -c "set title titlestring=$TITLE" ${LFile} ${RFile}
	fi
fi
