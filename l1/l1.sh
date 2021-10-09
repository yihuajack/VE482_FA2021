#!/bin/zsh
r=$RANDOM
while read INPUT
do
    if [ $INPUT -gt $r ]
    then
        echo "Smaller"
    elif [ $INPUT -lt $r ]
    then
        echo "Larger"
    else
        break
    fi
done