#!/bin/bash

n=16
k=16
programPath=$1
countWrong=0
count=0

while [ $n -lt 100000 ]; do
  echo Size: $n
  echo Time: $(date)
  # create the matrices, compute the product
  # ~/Matrix/matrix1.txt, matrix2.txt
  # ~/Utils/product.txt
  python3 ../Utils/augmentedSparse.py $n
  # prettify that product that got produced`
  ../Utils/prettify ../Utils/product.txt
  $programPath > junk

  DIFF=$(diff junk ../Utils/product.txt --ignore-space-change)
  if [ "$DIFF" != "" ]
  then
 #   let countWrong=$countWrong+1
    
    echo "☠️  Output is WRONG"
    echo "Quadtree:"
    cat junk
    echo "Python:"
    cat ../Utils/product.txt
  else
    echo "✅ matching"
  fi
  let n=$n*2
#  let count=$count+1
done

echo "$countWrong/$count were wrong"
