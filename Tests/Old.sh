#!/bin/bash

n=64

while [ $n -lt 10000 ]; do
  echo Size: $n
  # call augmentedSparse : This will write the matrices to disk
  ../Utils/augmentedSparse.py $n
  # now prettify the write to disk that was performed
  ../Utils/prettify product.txt

  # needs to execute and say how long it took
  time ../OldQuadTree/BetterQuadTree > junk1
  DIFF=$(diff junk1 product.txt)
  if [ "$DIFF" != "" ]
  then
    echo "ERROR: THEY DO NOT MATCH!!☠️  ☠️  ☠️  "
    echo $DIFF
  else
    echo "✅ matching"
  fi
  echo "********************"
  let n=2*$n
done

