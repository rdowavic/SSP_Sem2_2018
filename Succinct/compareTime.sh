#!/bin/bash

n=64

while [ $n -lt 10000 ]; do
  echo Size: $n
  # shove the matrix into matrix.txt
  python3 randomSparseMatrix.py $n > ~/Matrix/matrix.txt
  # needs to execute and say how long it took
  echo bit mult:
  time ./bitMatrixLeaf/Succinct > junk
  echo vanilla:
  time ./Succinct > junk
  echo "********************"
  let n=2*$n
done

