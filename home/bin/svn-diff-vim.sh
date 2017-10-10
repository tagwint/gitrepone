#!/bin/bash
# echo Parameters passed to merge-tool: $*
# base=${1?1st argument is 'base' file}
# theirs=${2?2nd argument is 'theirs' file}
# mine=${3?3rd argument is 'mine' file}
# merged=${4?4th argument is 'merged' file}
# version=$(meld --version | perl -pe '($_)=/([0-9]+([.][0-9]+)+)/' )    
echo '@' Parameters passed to merge-tool: "$@"
echo '*' Parameters passed to merge-tool: "$*"
LLabel=${3}
RLabel=${5}
LFile=${6}
RFile=${7}
# version=$(meld --version | perl -pe '($_)=/([0-9]+([.][0-9]+)+)/' )    

# echo base is ____ $base 
# echo theirs is __ $theirs
# echo mine is ____ $mine
# echo merged _____ $merged
# echo version ____ $version    
# echo ---------------
# echo "$@"
# echo ---------------
# meld --label="$LLabel"    "$LFile"   \
#      --label="$RLabel"    "$RFile"   
TITLE="${5} - ${3}"
TITLE=$(echo ${TITLE//(/\\(})
TITLE=$(echo ${TITLE//)/\\)})
TITLE=$(echo ${TITLE// /\\ })
TITLE=$(echo ${TITLE//-/\\-})
if [ "$TMUX" ] 
then 
  tmux new-window -n  "svndiff"  /usr/bin/vimdiff -c "set title titlestring=$TITLE" ${7} ${6}
else 
  /usr/bin/vimdiff -c "set title titlestring=$TITLE" ${7} ${6}
fi

