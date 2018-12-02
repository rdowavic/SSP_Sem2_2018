#!/bin/bash

failed=0

for i in {1..10}
do
  python3 randomSparseMatrix.py $1 > matrix.txt
  DIFF=(diff <(./BetterQuadTreeMultBits) <(~/BetterQuadTreeMult/BetterQuadTreeMult))
  if [ "$DIFF" ]
  then
    ((failed++))
  fi
done

echo "$failed/50 tests failed!"
