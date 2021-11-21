#!/bin/zsh
FILE=ex3-2.txt
if ! test -f "$FILE";
then
    echo "0" >> $FILE
fi
for i in {1..100}
do
(
    # -e, -x, --exclusive
    # Obtain an exclusive lock, sometimes called a write lock.  This is the default.
    # https://stackoverflow.com/questions/24388009/linux-flock-how-to-just-lock-a-file
    flock -n 3
    if [ $? -eq 1 ];
    then
        exit;
    fi
    number=$(tail -1 $FILE)
    ((++number))
    echo $number >> $FILE
) 3>>$FILE
done
