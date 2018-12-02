#include "SuccinctTree.h"

SuccinctTree::SuccinctTree(size_t dim) : dimension(dim) {}
// let the constructor/destructor just be default for now, in fact
// I might be able to just leave it like that
SuccinctTree SuccinctTree::Construct(std::string s) {
  SuccinctTree tree;
  // figure out dimensionality from the string size
  MatrixProcessor p;
  std::string cleaned = p.strip(s);
  assert(cleaned.length() % 4 == 0);
  tree.dimension = (size_t) sqrt(cleaned.length());
  tree.insert(cleaned);
  return tree;
}

bool SuccinctTree::empty() const {
  return ruler.empty();
}

void SuccinctTree::giveChild() {
  ruler.resize(ruler.size() + NUM_CHILDREN);
}

std::string SuccinctTree::meatAndBones() const {
  std::stringstream ss;

  for (size_t i = 0; i < ruler.size(); ++i) {
    if (i % 4 == 0)
      ss << "|";
    ss << ruler[i] << " ";
  }

  ss << "|";
  return ss.str();
}

std::ostream& operator <<(std::ostream& stream, const SuccinctTree& t) {
  stream << "[";
  for (size_t i = 0; i < t.dimension; ++i) {
    for (size_t j = 0; j < t.dimension; ++j) {
      stream << t.at(i, j);
    }
    if (i != t.dimension - 1) stream << ", ";
  }
  stream << "]";
  return stream;
}

SuccinctTree SuccinctTree::operator*(const SuccinctTree& other) {
  return Product(other);
}

SuccinctTree SuccinctTree::operator+(const SuccinctTree& other) {
  return Union(other);
}

SuccinctTree SuccinctTree::Product(const SuccinctTree& other) const {
  assert(dimension == other.dimension);
  return Product(*this, HEAD_NODE, other, HEAD_NODE, dimension);
}

SuccinctTree SuccinctTree::Union(const SuccinctTree& other) const {
  assert(dimension == other.dimension);
  return Union(*this, HEAD_NODE, other, HEAD_NODE, dimension);
}

SuccinctTree SuccinctTree::Union(const SuccinctTree& A, size_t nodeA,
                          const SuccinctTree& B, size_t nodeB, size_t dim) {

    if (nodeA == A.ruler.size()) return B;
    if (nodeB == B.ruler.size()) return A;

    // we're not empty so we can put a result
    SuccinctTree result(dim); result.giveChild();
    /**
    * We want to traverse over all subnodes in our two nodes
    * so we can join them up
    */
    for (size_t i = 0; i < NUM_CHILDREN; ++i) {
      uint32_t a = A.get(nodeA, i), b = B.get(nodeB, i);

      if (a == EMPTY && b == EMPTY) {
        result.set(HEAD_NODE, i, EMPTY);
      }
      else {
        result.set(HEAD_NODE, i, result.nextAvailable());
        if (a != EMPTY && b == EMPTY) {
          result.giveSubtree(A, a);
        }
        else if (a == EMPTY && b != EMPTY) {
          result.giveSubtree(B, b);
        }
        else {
          // they're both non-zero and we need to merge them
          SuccinctTree temp = Union(A, a, B, b, dim / 2);
          result.giveSubtree(temp, HEAD_NODE);
        }
      }
    } // for loop is over
    return result;
}

SuccinctTree SuccinctTree::Product(const SuccinctTree& A, size_t nodeA,
                          const SuccinctTree& B, size_t nodeB, size_t dim) {
    SuccinctTree result(dim);

    if (A.ruler.size() == nodeA || B.ruler.size() == nodeB)
      return result; // if one of them is zero, the product is 0

    result.giveChild();

    for (size_t i = 0; i < ROW_SIZE; ++i) {
      for (size_t j = 0; j < ROW_SIZE; ++j) {
        SuccinctTree temp(dim / 2);
        for (size_t k = 0; k < ROW_SIZE; ++k) {
          uint32_t first = A.get(nodeA, i, k);
          uint32_t second = B.get(nodeB, k, j);
          //we want to compute the product of these two if we can
          if (first != EMPTY && second != EMPTY) {
            SuccinctTree product = Product(A, first, B, second, dim / 2);
            temp = temp + product;
          }
        }
        // we have now figured out temp. where does temp need to go?
        // in the i, jth entry
        if (temp.empty())
          result.set(HEAD_NODE, i, j, EMPTY);
        else {
          result.set(HEAD_NODE, i, j, result.nextAvailable());
          result.giveSubtree(temp, HEAD_NODE);
        }
      }
    }
    return result;
}

void SuccinctTree::clear() {
  std::fill(ruler.begin(), ruler.end(), EMPTY);
}

void SuccinctTree::giveSubtree(const SuccinctTree& from, size_t nodeNumber) {
  // so we look at our next available spot and stick it all there,
  // but we just have to shift the numbers up
  // if it's empty maybe then just make an early exit
  if (nodeNumber == from.ruler.size())
    return;
  size_t offset = nextAvailable(); giveChild();

  for (size_t i = 0; i < NUM_CHILDREN; ++i) {
    if (from.get(nodeNumber, i)) {
      // set the value
      set(offset, i, nextAvailable());
      // then give the appropriate stuff for that value
      giveSubtree(from, from.get(nodeNumber, i));
    } else {
      set(offset, i, EMPTY);
    }
  }
}

size_t SuccinctTree::nextAvailable() const {
  return ruler.size();
}

uint32_t SuccinctTree::get(size_t nodeNumber, size_t i, size_t j) const {
  // gets this quadrant out, just does the multiplication by
  // ROW_SIZE for the user
  return ruler[nodeNumber + (ROW_SIZE * i + j)];
}

void SuccinctTree::set(size_t nodeNumber, size_t i, size_t j, uint32_t value) {
  ruler[nodeNumber + (ROW_SIZE * i + j)] = value;
}

void SuccinctTree::set(size_t nodeNumber, uint32_t entry, uint32_t value) {
  ruler[nodeNumber + entry] = value;
}

uint32_t SuccinctTree::get(size_t nodeNumber, uint32_t entry) const {
  // you have to give 0 <= entry <= 3
  // node number has to be less than length / 4
  assert(nodeNumber < ruler.size());
  assert(entry >= 0 && entry < 4);
  return ruler[nodeNumber + entry];
}

bool SuccinctTree::at(size_t i, size_t j) const {
  // deal with the case where the tree is empty
  if (ruler.empty()) return false;
  // base case is when dimension == 1 (a 1 by 1 matrix)?
  size_t dim = dimension;
  size_t quadrant;

  for (size_t k = 0; dim > 1; k = ruler[k + quadrant], dim /= 2) {
    quadrant = quadrantNum(i, j, dim);
    if (ruler[k + quadrant] == EMPTY)
      return false;
    // don't we need to also adjust the quadrant variable?
    if (quadrant >= 2) i -= dim / 2;
    if (quadrant % 2 == 1) j -= dim / 2;
  }

  return true;
}

size_t SuccinctTree::quadrantNum(size_t i, size_t j, size_t dim) const {
   int quadNum = 0;
   if (i >= dim / 2) quadNum += 2;
   if (j >= dim / 2) quadNum++;
   return quadNum;
}


int SuccinctTree::insert(std::string child) {
  // in the base case, we need to return 0
  if (child.find('1') == std::string::npos)
    return 0;

  // need to make new item at the end
  int index = ruler.size(); // will be zero the first time
  // add 4 new entries to the back of this vector
  giveChild();
  // with meaningful values in the loop below
  MatrixProcessor p;

  for (int i = 0; i < 4; ++i) {
    std::string grandChild = p.recoverQuadrant(child, i);
    ruler[index + i] = insert(grandChild);
  }
  return index;
}
