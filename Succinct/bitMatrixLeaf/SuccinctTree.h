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
#include "bitmult.h"

// list of constants that are facts about the dimensions,
// nature of the matrix etc
// ie, every non-leaf always has 4 children
const int NUM_CHILDREN = 4;
// every matrix has 'two rows' of submatrices
const int ROW_SIZE = 2;
const int HEAD_NODE = 0;
const int EMPTY = 0;
// for the Bit version I need to do some new ones for the 8x8 idea
const int LEAF_DIM = 8;
constexpr int LEAF_MUM = 2 * LEAF_DIM;
constexpr int LEAF_SIZE = LEAF_DIM * LEAF_DIM;

typedef size_t Node;

class SuccinctTree {
public:
  /**
   * meatAndBones gives a string representation of the
   * underlying data structure
   */
  std::string meatAndBones() const;
  std::string str();
  /**
   * SuccinctTree(size_t) instantiates a new SuccinctTree
   * with the given dimension
   */
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
  /**
   * This computes the union, so if A and B are two
   * SuccinctTrees, then A + B is the union of A and B.
   */
  SuccinctTree operator+(const SuccinctTree& other);

  /*
  * These are for Union, Product, etc
  */
private:
  SuccinctTree Union(const SuccinctTree& other) const;
  static SuccinctTree Union(const SuccinctTree& A, size_t startA,
                            const SuccinctTree& B, size_t startB, size_t dim);

  SuccinctTree Product(const SuccinctTree& other) const;
  static SuccinctTree Product(const SuccinctTree& A, Node nodeA,
                            const SuccinctTree& B, Node nodeB, size_t dim);

  void giveSubtree(const SuccinctTree& from, Node startNode, size_t length);
  // gives you the pointer to the next available slot in memory
  size_t get(Node node, size_t i, size_t j) const;
  size_t get(Node node, size_t entry) const;
  size_t getLength(Node node, size_t entry) const;
  void set(Node node, size_t i, size_t j, size_t value);
  void set(Node node, size_t entry, size_t value);
  void clear();

  Node next(Node n) const;

  /**
  * insert constructs the child as necessary and then
  * returns its index so that the current index knows how to
  * refer to it. This index will always be the length of the vector
  * when the call is first made.
  */
  size_t insert(std::string child);
  /**
  * giveChild adds four new entries to the SuccinctTree's ruler
  */
  void giveChild();
  /**
  * empty checks if the SuccinctTree is empty (it has no entries at all)
  */
  bool empty() const;
  /**
  * at retrieves the boolean stored at this specific index in the matrix
  */
public:
  bool at(size_t i, size_t j) const;
  size_t quadrantNum(size_t i, size_t j, size_t dim) const;
  size_t jumpSize(size_t position) const;

public:
  std::vector<size_t> ruler;
  size_t dimension;
};
