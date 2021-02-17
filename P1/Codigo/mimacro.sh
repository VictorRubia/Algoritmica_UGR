#!/bin/bash
echo "" > salida.dat
let i=1000
while [ $i -le 10000 ];do
  echo $i
  ./burbuja  $i >> salida.dat
  let i=$i+1000
done
