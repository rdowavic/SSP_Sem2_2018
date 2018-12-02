#!/bin/bash

n=2
programPath=$1

while [ $n -lt 10000 ]; do
  echo Size: $n
  # create the matrices, compute the product
  # ~/Matrix/matrix1.txt, matrix2.txt
  # ~/Utils/product.txt
  python3 ../Utils/augmentedUnion.py $n   
  # prettify that product that got produced`
  ../Utils/prettify ../Utils/union.txt
  $programPath > junk

  DIFF=$(diff junk ../Utils/union.txt)
  if [ "$DIFF" != "" ]
  then
    echo "☠️  Output is WRONG"
    echo $DIFF
  else
    echo "✅ "
  fi
  let n=2*$n
done

