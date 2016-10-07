#!/bin/bash

# variables
ROW_AVG=0
COUNT=0

# Check the argument length and set file

if (( $# > 2 ))
then
        exit 1
fi

if [[ $# = 1 ]]
then
        FILE=$1
else
        FILE=$2
fi

# Check if file is empty
if [ ! -s $FILE ]
then
        exit 1
fi

# Check for rows flag or default 1 argument
if [[ $1 = -r* ]] || [[ $# = 1 ]]
then
        echo "AVG MEDIAN"
        # check each line
        while read line
        do
                for num in $line
                do
                        ROW_AVG=`expr $ROW_AVG + $num`
                        COUNT=`expr $COUNT + 1`
                done
                echo $line > tmp # push the line into a file
                tr ' ' '\n' < tmp > tmp1 # add new line charaters
                sort -n -k 1 tmp1 > tmp # sorts list and moves back to tmp
                DENOM=`expr $COUNT / 2`
                if [ `expr $COUNT % 2` = 1 ]
                then
                        input=`expr $DENOM + 1` # stores row for median in even number
                        MEDIAN="$(sed ${input}q tmp | tail -1)" # pulls out the median
                else
                        input=`expr $DENOM + 1`
                        x="$(sed ${input}q tmp | tail -1)" # pulls out the median
                        y="$(sed ${DENOM}q tmp | tail -1)" # pulls out the median
                        MEDIAN=`expr $(($x + $y + 1)) / 2` # rounds to nearest
                fi
                ROW_AVG=`expr $(($ROW_AVG + $DENOM))  / $COUNT` # calc avg
                echo "$ROW_AVG $MEDIAN"
                ROW_AVG=0
                COUNT=0
        done < $FILE
elif [[ $1 = -c* ]]
then
        echo "normie"
fi
