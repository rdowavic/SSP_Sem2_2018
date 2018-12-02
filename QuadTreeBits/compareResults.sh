#!/bin/bash

n=64

while [ $n -lt 513 ]; do
  # shove the matrix into matrix.txt
  python3 randomSparseMatrix.py $n > matrix.txt
  # needs to execute and check that both are the same
  # echo time ./BetterQuadTreeMultBits
  DIFF=$(diff <(~/BetterQuadTreeMult/BetterQuadTreeMult) <(~/BetterQuadTreeBits/BetterQuadTreeMultBits))
  if [ "$DIFF" != "" ]
  then
    echo "Different on size $n"
    echo $DIFF | tail -5
  else
    echo "Same on size $n (TICK)"
  fi
  let n=2*$n
done

