#!/bin/bash
SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""


S21="./s21_grep"
SYS="grep"
E_SYS="egrep"


FLAGS=("v" "c" "l" "n" "h" "o")

PATS=("for" "while" "if" "case" "int" "intel" "s" "void" "a" "1")
PAT_FILE="test_files/test_ptrn_grep.txt"
EXT_PAT=("[a-z]" "[a-z]{2,5}" "[0-9]{0,1}" ";$" "^;$" "^int")

FILES=($(find ./$TEST_DIR -name "*.txt" -type f) "not_file.txt")

make rebuild

echo ""
echo "One arg, file pattern, one file"
echo ""

for arg1 in ${FLAGS[*]}
do
    for file1 in ${FILES[*]}
    do
        $S21 -$arg1 -f $PAT_FILE $file1 > $S21.log
        $E_SYS -$arg1 -f $PAT_FILE $file1 > $SYS.log
        DIFF=$(diff -s $S21.log $SYS.log)
        let "COUNTER++"
        if [ "$DIFF" == "Files $S21.log and $SYS.log are identical" ]
        then
            let "SUCCESS++"
            echo "$COUNTER - Success: -$arg1 -f $PAT_FILE $file1"
        else
            let "FAIL++"
            echo "$COUNTER - Fail: -$arg1 -f $PAT_FILE $file1"
        fi
    done
done

echo "One pattern, one file"
echo ""

for file in ${FILES[*]}
do
    for pat in ${PATS[*]}
    do
        $S21 $pat $file > $S21.log
        $SYS $pat $file > $SYS.log
        DIFF=$(diff -s $S21.log $SYS.log)
        let "COUNTER++"
        if [ "$DIFF" == "Files $S21.log and $SYS.log are identical" ]
        then
            let "SUCCESS++"
            echo "$COUNTER - Success: $pat $file"
        else
            let "FAIL++"
            echo "$COUNTER - Fail: $pat $file"
        fi
    done
done

echo ""
echo "Two patterns, one file"
echo ""


for file in ${FILES[*]}
do
    for pat1 in ${PATS[*]}
    do
        for pat2 in ${PATS[*]}
        do
            $S21 -e $pat1 -e $pat2 $file > $S21.log
            $E_SYS -e $pat1 -e $pat2 $file > $SYS.log
            DIFF=$(diff -s $S21.log $SYS.log)
            let "COUNTER++"
            if [ "$DIFF" == "Files $S21.log and $SYS.log are identical" ]
            then
                let "SUCCESS++"
                echo "$COUNTER - Success: -e $pat1 -e $pat2 $file"
            else
                let "FAIL++"
                echo "$COUNTER - Fail: -e $pat1 -e $pat2 $file"
            fi
        done
    done
done

echo ""
echo "Two args, one pattern, one file"
echo ""

for file in ${FILES[*]}
do
    for pat in ${PATS[*]}
    do
        for arg1 in ${FLAGS[*]}
        do
            for arg2 in ${FLAGS[*]}
            do
                if [ $arg1 != $arg2 ]
                then
                    $S21 -$arg1 -$arg2 $pat $file > $S21.log
                    $SYS -$arg1 -$arg2 $pat $file > $SYS.log
                    DIFF=$(diff -s $S21.log $SYS.log)
                    let "COUNTER++"
                    if [ "$DIFF" == "Files $S21.log and $SYS.log are identical" ]
                    then
                        let "SUCCESS++"
                        echo "$COUNTER - Success: -$arg1 -$arg2 $pat $file"
                    else
                        let "FAIL++"
                        echo "$COUNTER - Fail: -$arg1 -$arg2 $pat $file"
                    fi
                fi
            done
        done
    done
done

echo ""
echo "Triple args, one external pattern, one file"
echo ""

for arg3 in ${FLAGS[*]}
do
    for file1 in ${FILES[*]}
    do
        for pat1 in ${EXT_PAT[*]}
        do
            for arg1 in ${FLAGS[*]}
            do
                for arg2 in ${FLAGS[*]}
                do
                    if [ $arg1 != $arg2 ] && [ $file1 != $file2 ] && [ $pat1 != $pat2 ]  && [ $arg1 != $arg3 ] && [ $arg2 != $arg3 ]
                    then
                        $S21 -$arg1$arg2$arg3 -e $pat1 $file1 > $S21.log
                        $E_SYS -$arg1$arg2$arg3 -e $pat1 $file1 > $SYS.log
                        DIFF=$(diff -s $S21.log $SYS.log)
                        let "COUNTER++"
                        if [ "$DIFF" == "Files $S21.log and $SYS.log are identical" ]
                        then
                            let "SUCCESS++"
                            echo "$COUNTER - Success: -$arg1$arg2$arg3 -e $pat1 $file1"
                        else
                            let "FAIL++"
                            echo "$COUNTER - Fail: -$arg1$arg2$arg3 -e $pat1 $file1"
                        fi
                    fi
                done
            done
        done
    done
done

echo ""
echo "Two args, one pattern, two files"
echo ""

for file2 in ${FILES[*]}
do
    for file1 in ${FILES[*]}
    do
        for pat in ${PATS[*]}
        do
            for arg1 in ${FLAGS[*]}
            do
                for arg2 in ${FLAGS[*]}
                do
                    if [ $arg1 != $arg2 ] && [ $file1 != $file2 ]
                    then
                        $S21 -$arg1 -$arg2 $pat $file1 $file2 > $S21.log
                        $SYS -$arg1 -$arg2 $pat $file1 $file2 > $SYS.log
                        DIFF=$(diff -s $S21.log $SYS.log)
                        let "COUNTER++"
                        if [ "$DIFF" == "Files $S21.log and $SYS.log are identical" ]
                        then
                            let "SUCCESS++"
                            echo "$COUNTER - Success: -$arg1 -$arg2 $pat $file1 $file2"
                        else
                            let "FAIL++"
                            echo "$COUNTER - Fail: -$arg1 -$arg2 $pat $file1 $file2"
                        fi
                    fi
                done
            done
        done
    done
done

echo ""
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
echo ""