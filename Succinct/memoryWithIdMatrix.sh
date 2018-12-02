#!/bin/bash

## try with matrix sizes under 1024
n=64

while [ $n -le 4097 ]; do
    python3 randomSparseMatrix.py $n > matrix.txt
    echo ID matrix with size $n
    echo Bit matrix:
    ./BetterQuadTreeMultBits
    echo Vanilla:
    ~/OldQuadTree/BetterQuadTree
    let n=2*$n
done
