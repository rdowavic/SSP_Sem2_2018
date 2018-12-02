#include <vector>
#include "succincttree.h"
class SuccinctTree;
/**
 * Multiplier serves as a plug-and-play for SuccinctTrees 
 */
class Multiplier {
public:
  Multiplier() = default;
  /**
  * Union gives back a SuccinctTree that is the union of A and B. 
  */
  SuccinctTree Union(SuccinctTree& A, SuccinctTree& B);
  /**
  * Product computes the product of A and B as boolean matrices
  * and gives the result back wrapped up in a new SuccinctTree.
  */
  SuccinctTree Product(SuccinctTree& A, SuccinctTree& B);
  /**
  * giveSubtree copies the subtree starting at startIndex from from to to 
  * (which does not contain that subtree).
  */
private:
  SuccinctTree Union(SuccinctTree& A, size_t startA,
                     SuccinctTree& B, size_t startB);

  SuccinctTree Product(SuccinctTree& A, size_t startA,
                       SuccinctTree& B, size_t startB);

  void giveSubtree(SuccinctTree& to, size_t startIndex, SuccinctTree& from);
  /**
  * mergeSubtree merges the subtrees starting at startA and startB from A
  * and B respectively and stores the result in dest at the next available
  * location in memory.
  */
  void mergeSubtree(SuccinctTree& dest, SuccinctTree& A, 
                    SuccinctTree& B, size_t startA, size_t startB);
public:
  ~Multipler() = default;
 
};
