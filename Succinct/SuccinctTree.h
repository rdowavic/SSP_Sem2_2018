#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <stdint.h>
#include <cmath>
#include <cassert>
#include "matrixprocessor.h"
#include <string>
#include <sstream>

// list of constants that are facts about the dimensions,
// nature of the matrix etc
// ie, every non-leaf always has 4 children
const int NUM_CHILDREN = 4;
// every matrix has 'two rows' of submatrices
const int ROW_SIZE = 2;
const int HEAD_NODE = 0;
const int EMPTY = 0;

class SuccinctTree {
public:
  std::string meatAndBones() const;
  SuccinctTree(size_t dim);
  SuccinctTree() = default;
  SuccinctTree(const SuccinctTree& t) = default;
  ~SuccinctTree() = default;
  /**
   * Construct is a factory method that takes a
   * string s and constructs a Succinct Quadtree
   * matrix from it.
   */
  static SuccinctTree Construct(std::string s);
  /**
  * empty checks if this instance of SuccinctTree
  * is empty
  */
  friend std::ostream& operator <<(std::ostream& stream, const SuccinctTree& t);
  /**
  * A * B computes the matrix product of the trees A and B
  */
  SuccinctTree operator*(const SuccinctTree& other);
  SuccinctTree operator+(const SuccinctTree& other);

  /*
  * These are for Union, Product, etc
  */
private:
  SuccinctTree Union(const SuccinctTree& other) const;
  static SuccinctTree Union(const SuccinctTree& A, size_t startA,
                            const SuccinctTree& B, size_t startB, size_t dim);

  SuccinctTree Product(const SuccinctTree& other) const;
  static SuccinctTree Product(const SuccinctTree& A, size_t startA,
                            const SuccinctTree& B, size_t startB, size_t dim);

  void giveSubtree(const SuccinctTree& from, size_t nodeNumber);
  // gives you the pointer to the next available slot in memory
  size_t nextAvailable() const;
  uint32_t get(size_t nodeNumber, size_t i, size_t j) const;
  uint32_t get(size_t nodeNumber, uint32_t entry) const;
  void set(size_t nodeNumber, size_t i, size_t j, uint32_t value);
  void set(size_t nodeNumber, uint32_t entry, uint32_t value);
  void clear();
  /**
  * insert constructs the child as necessary and then
  * returns its index so that the current index knows how to
  * refer to it. This index will always be the length of the vector
  * when the call is first made.
  */
  int insert(std::string child);
  /**
  * giveChild adds four new entries to the SuccinctTree's ruler
  */
  void giveChild();
  /**
  * empty checks if the SuccinctTree is empty
  */
  bool empty() const;
  /**
  * at retrieves the boolean stored at this specific index in the matrix
  */
  bool at(size_t i, size_t j) const;
  size_t quadrantNum(size_t i, size_t j, size_t dim) const;

public:
  std::vector<uint32_t> ruler;
  size_t dimension;
};
