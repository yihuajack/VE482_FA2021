#!/bin/zsh
FILE=ex3-1.txt
# CNT=100
if ! test -f "$FILE";
then
    echo 0 >> $FILE
fi
for i in {1..100}
do
    number=$(tail -1 $FILE)
    ((++number))
    echo $number >> $FILE
done
