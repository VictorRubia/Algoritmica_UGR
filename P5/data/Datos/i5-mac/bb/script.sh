#!/bin/bash
FILES="./a280/a280_i7_2.txt
./a280/a280_i7_3.txt
./a280/a280_i7_4.txt
./a280/a280_i7_5.txt
./a280/a280_i7_6.txt
./a280/a280_i7_7.txt
./a280/a280_i7_8.txt
./a280/a280_i7_9.txt
./a280/a280_i7_10.txt
./a280/a280_i7_11.txt
./a280/a280_i7_12.txt"
for f in $FILES
do
cat $f >> a280.txt
done