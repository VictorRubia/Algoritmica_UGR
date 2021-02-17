#!/bin/bash
FILES="./uly16/uly16_i7_2.txt
./uly16/uly16_i7_3.txt
./uly16/uly16_i7_4.txt
./uly16/uly16_i7_5.txt
./uly16/uly16_i7_6.txt
./uly16/uly16_i7_7.txt
./uly16/uly16_i7_8.txt
./uly16/uly16_i7_9.txt
./uly16/uly16_i7_10.txt
./uly16/uly16_i7_11.txt
./uly16/uly16_i7_12.txt"
for f in $FILES
do
cat $f >> uly16.txt
done