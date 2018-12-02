#include "Multiplier.h"

class SuccinctTree;

SuccinctTree SuccinctTree::Union(SuccinctTree& A, SuccinctTree& B) {
  // handle trivial cases where one is empty
  if (A.empty()) return B;
  if (B.empty()) return A;
  // now start copying from the children and
  // do recursive stuff after that
  SuccinctTree result;
  mergeSubtree(result, A, B, 0, 0);
  return result;
}

SuccinctTree SuccinctTree::Product(SuccinctTree& A, SuccinctTree& B) {
  // if one of them is empty the whole thing is empty
  SuccinctTree result;
  if (A.empty() || B.empty()) 
    return result;
  getProduct(result, A, B, 0, 0);  
  return result;
}

void SuccinctTree::getProduct(SuccinctTree& dest, SuccinctTree& A,
                              SuccinctTree& B, size_t startA, 
                              size_t startB) {
  size_t startDest = dest.ruler.size();
  dest.giveChild();

  for (size_t i = 0; i < ROW_SIZE; ++i) {
    for (size_t j = 0; j < ROW_SIZE; ++j) {
       SuccinctTree temp;
       temp.giveChild();
       // make all the entries 0 by default
       for (size_t m = 0; m < NUM_CHILDREN; ++m) 
         temp.ruler[m] = 0;
       for (size_t k = 0; k < ROW_SIZE; ++k) {
          // get the right locations for A and B
          size_t first = A.ruler[startA + (ROW_SIZE * i + k)];
          size_t second = B.ruler[startB + (ROW_SIZE * k + j)];
          // check if either are 0
          if (first && second) {
              // then go and calculate their product
              SuccinctTree product;
              getProduct(product, A, B, first, second); 
              // then merge with t
              temp = Union(temp, product);
          }
       }
      // out of the innermost loop now,
      // I have a child to give
      dest.ruler[startDest + (ROW_SIZE * i + j)] = dest.ruler.size();
      startDest++;
      giveSubtree(dest, 0, temp);
    }
  }
}

void SuccinctTree::mergeSubtree(SuccinctTree& dest, SuccinctTree& A, 
                    SuccinctTree& B, size_t startA, size_t startB) {
  
  size_t startDest = dest.ruler.size();
  dest.giveChild();
  
  for (size_t i = 0; i < NUM_CHILDREN; ++i) {
    // if one of them is empty and the
    // other isn't
    if (!A.ruler[i + startA] ^ !B.ruler[i + startB]) {
      // figure out which one isn't empty and giveSubtree
      // from it
      SuccinctTree& from = A.ruler[i + startA] ? A : B;
      size_t offset = A.ruler[i + startA] ?
                        A.ruler[i + startA] :
                        B.ruler[i + startB];
      giveSubtree(dest, offset, from);
    }
    // else see if you have to merge the two
    else if (A.ruler[i + startA] && B.ruler[i + startB]) {
      dest.ruler[i + startDest] = dest.ruler.size();
      mergeSubtree(dest, A, B, A.ruler[i + startA], 
                   B.ruler[i + startB]);
    }
    else {
      dest.ruler[i + startDest] = 0;
    }
  }
}

void SuccinctTree::giveSubtree(SuccinctTree& to, size_t startIndex, 
                               SuccinctTree& from) {
  // how do you give a subtree?? Hmmmm
  size_t indexTo = to.ruler.size();
  to.giveChild();
  size_t j = startIndex;
  for (size_t i = indexTo; i < indexTo + NUM_CHILDREN; ++i, ++j) {
    if (from.ruler[j]) {
      to.ruler[i] = to.ruler.size();
      giveSubtree(to, from.ruler[j], from);
    } else
      to.ruler[i] = 0;
  }
}

