#!/bin/bash

TEST_DIR=test
WORK_DIR=test/cmp_files
CAT_FILE=cat
MY_FILE=file
FLAGS=(-b -e -n -s -t -v -E -T -bn -be -se -st -ET -nst -best --number-nonblank --number --squeeze-blank --show-tabs --show-ends --show-nonprinting -benstv)

make test

chmod 777 -R $TEST_DIR

FILES=$(find ./$TEST_DIR -name "*.txt" -type f)

echo ""
echo "Test with no flags"

for name in $FILES
do
    cat $name > $WORK_DIR/$CAT_FILE
    ./s21_cat $name > $WORK_DIR/$MY_FILE
    if (cmp $WORK_DIR/$CAT_FILE $WORK_DIR/$MY_FILE &> /dev/null)
        then echo "Equal" >> $WORK_DIR/result.txt
        else echo "Diff" >> $WORK_DIR/result.txt && break 
    fi
done

echo ""
echo "Test with flags"

for flag in ${FLAGS[*]}
do
    echo ""
    echo "Flag is \"$flag\""
    for name in $FILES
    do
        cat $flag $name > $WORK_DIR/$CAT_FILE
        ./s21_cat $flag $name > $WORK_DIR/$MY_FILE
        if (cmp $WORK_DIR/$CAT_FILE $WORK_DIR/$MY_FILE &> /dev/null)
            then echo "Equal" >> $WORK_DIR/result.txt
            else echo "Diff" >> $WORK_DIR/result.txt && break
        fi
    done
done

echo
echo "Failed tests $(grep -c "Diff" $WORK_DIR/result.txt)"
echo