#!/bin/bash

n=64

while [ $n -lt 1025 ]; do
  echo Size: $n
  # shove the matrix into matrix.txt
  python3 idMatrix.py $n > matrix.txt
  # needs to execute and check that both are the same
  # echo time ./BetterQuadTreeMultBits
  DIFF=$(diff <(~/BetterQuadTreeBits/BetterQuadTreeMultBits) <(~/BetterQuadTreeMult/BetterQuadTreeMult)) 
  if [ "$DIFF" != "" ]
  then
    echo "Different on size $n"
  else
    echo "Passed on size $n (TICK)"
  fi

  let n=2*$n
done

