#!/bin/bash
# echo Parameters passed to merge-tool: $*
# base=${1?1st argument is 'base' file}
# theirs=${2?2nd argument is 'theirs' file}
# mine=${3?3rd argument is 'mine' file}
# merged=${4?4th argument is 'merged' file}
# version=$(meld --version | perl -pe '($_)=/([0-9]+([.][0-9]+)+)/' )    
# echo '@' Parameters passed to merge-tool: "$@"
# echo '*' Parameters passed to merge-tool: "$*"
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

sdiff -s ${LFile} ${RFile}
