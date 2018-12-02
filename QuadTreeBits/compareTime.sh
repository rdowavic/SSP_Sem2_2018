#!/bin/bash

n=64

while [ $n -lt 100000 ]; do
  echo Size: $n
  # shove the matrix into matrix1.txt and matrix2.txt
  python3 ../Utils/augmentedSparse.py $n
  # needs to execute and say how long it took
  echo length encoded:
  time ../Succinct/lengthEncoded/Succinct > junk
  echo bitmatrix length encoded:
  time ../Succinct/bitMatrixLeaf/Succinct > junk
  echo "********************"
  let n=2*$n
done

