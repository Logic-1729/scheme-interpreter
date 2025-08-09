#!/bin/bash

echo "This is a simple score shell script for you to find out problems in your program"
echo "--------------------------------------------------------------------------------"

# 确保我们在score目录下
cd "$(dirname "$0")"

L=1
R=118
for ((i = $L; i <= $R; i = i + 1))
do
    echo ""
    echo "---------------------------"
    echo "Ready to test: TEST" $i
    if [ ! -f "data/$i.in" ]; then
        echo "Input file data/$i.in not found, skipping TEST $i"
        continue
    fi
    if [ ! -f "data/$i.out" ]; then
        echo "Output file data/$i.out not found, skipping TEST $i"
        continue
    fi
    ../build/code << EOF > scm.out
    $(cat data/$i.in)
    (exit)
EOF
    sed '$d' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    sed 's/scm> //' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    diff -b scm.out data/$i.out > diff_output.txt
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
    if [ ! -f "more-tests/$i.in" ]; then
        echo "Input file more-tests/$i.in not found, skipping EXTRA TEST $i"
        continue
    fi
    if [ ! -f "more-tests/$i.out" ]; then
        echo "Output file more-tests/$i.out not found, skipping EXTRA TEST $i"
        continue
    fi
    ../build/code << EOF > scm.out
    $(cat more-tests/$i.in)
    (exit)
EOF
    sed '$d' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    sed 's/scm> //' scm.out > scm_cleaned.out
    mv scm_cleaned.out scm.out
    diff -b scm.out more-tests/$i.out > diff_output.txt
    if [ $? -ne 0 ]; then
        echo "Wrong answer in EXTRA TEST" $i
        # echo "---------------------------"
        # echo ""
        # exit 1
    fi
    echo "---------------------------"
    echo ""
done