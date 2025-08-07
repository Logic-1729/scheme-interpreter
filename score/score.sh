#!/bin/bash

echo "This is a simple score shell script for you to find out problems in your program"
echo "--------------------------------------------------------------------------------"

L=1
R=119
for ((i = $L; i <= $R; i = i + 1))
do
    echo ""
    echo "---------------------------"
    echo "Ready to test: TEST" $i
    ../code << EOF > scm.out
    $(cat ./data/$i.in)
    (exit)
EOF
    sed '$d' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    sed 's/scm> //' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    diff -b scm.out ./data/$i.out > diff_output.txt
    if [ $? -ne 0 ]; then
        echo "Wrong answer in TEST" $i
        # echo "---------------------------"
        # echo ""
        # exit 1
    fi
    echo "---------------------------"
    echo ""
done

L_EXTRA=1
R_EXTRA=7
for ((i = $L_EXTRA; i <= $R_EXTRA; i = i + 1))
do
    echo ""
    echo "---------------------------"
    echo "Ready to test: EXTRA TEST" $i
    ../code << EOF > scm.out
    $(cat ./more-tests/$i.in)
    (exit)
EOF
    sed '$d' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    sed 's/scm> //' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    diff -b scm.out ./more-tests/$i.out > diff_output.txt
    if [ $? -ne 0 ]; then
        echo "Wrong answer in EXTRA TEST" $i
        # echo "---------------------------"
        # echo ""
        # exit 1
    fi
    echo "---------------------------"
    echo ""
done