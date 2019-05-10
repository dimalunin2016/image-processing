#!/bin/bash
g++ binorizator.cpp -o img-display `pkg-config --cflags --libs opencv`
declare -a arr=("01" "02" "03" "04" "05" "06" "07" "08" "09" "10" "11" "12" "13" "14" "15")
for i in "${arr[@]}"
do
   ./img-display Dataset/$i.JPG Result/bradley_$i.png
done

