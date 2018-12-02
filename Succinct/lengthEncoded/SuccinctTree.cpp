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

    // I'm not sure we even need those base cases
    // okay we definitely do! LOL.
    if (A.empty()) return B;
    if (B.empty()) return A;

    SuccinctTree result(dim); result.giveChild();

    for (size_t i = 0; i < NUM_CHILDREN; ++i) {
      size_t lengthA = A.getLength(nodeA, i);
      size_t lengthB = B.getLength(nodeB, i);
      // the actual nodes
      Node a = A.get(nodeA, i), b = B.get(nodeB, i);

      size_t entry = 0;
      // both empty? do nothing
      // A EMPTY
      if (a == EMPTY && b != EMPTY) {
        result.giveSubtree(B, b, lengthB);
        entry = lengthB;
      }
      // B empty
      else if (b == EMPTY && a != EMPTY) {
        result.giveSubtree(A, a, lengthA);
        entry = lengthA;
      }
      // neither empty! Have to take the union recursively.
      else if (a != EMPTY && b != EMPTY) {
        SuccinctTree temp = Union(A, a, B, b, dim / 2);
        // they should do the union of the first submatrix
        result.giveSubtree(temp, HEAD_NODE, temp.ruler.size());
        entry = temp.ruler.size();
      }
      // set the entry appropriately
      result.set(HEAD_NODE, i, entry);
    }

    return result;
}

SuccinctTree SuccinctTree::Product(const SuccinctTree& A, Node nodeA,
                          const SuccinctTree& B, Node nodeB, size_t dim) {
    // make sure neither of A or B is empty! Then just return an empty result.
    SuccinctTree result(dim);

    if (A.empty() || B.empty()) return result;

    result.giveChild();

    for (size_t i = 0; i < ROW_SIZE; ++i) {
      for (size_t j = 0; j < ROW_SIZE; ++j) {
        // temp stands in for result[i, j]
        SuccinctTree temp(dim / 2);
        for (size_t k = 0; k < ROW_SIZE; ++k) {
          size_t first = A.get(nodeA, i, k);
          size_t second = B.get(nodeB, k, j);
          //we want to compute the product of these two if we can
          if (first != EMPTY && second != EMPTY) {
            SuccinctTree product = Product(A, first, B, second, dim / 2);
            temp = temp + product;
          }
        }
        // set the thing to be temp.ruler.size(), then if it's not empty we'll give
        // the subtree
        result.set(HEAD_NODE, i, j, temp.ruler.size());
        // if it's not empty, then copy over the subtree
        if (!temp.empty()) {
          result.giveSubtree(temp, HEAD_NODE, temp.ruler.size());
        }
      } // end of the j loop
    } // end of the i loop

    return result;
}

void SuccinctTree::clear() {
  std::fill(ruler.begin(), ruler.end(), EMPTY);
}

/**
*
*
*
**/

void SuccinctTree::giveSubtree(const SuccinctTree& from, Node startNode, size_t length) {
  // we're going to copy all the way to here
  size_t end = startNode + length;
  ruler.insert(ruler.end(), from.ruler.begin() + startNode, from.ruler.begin() + end);
}

Node SuccinctTree::next(Node n) const {
  assert(n % NUM_CHILDREN == 0);
  return n + NUM_CHILDREN;
}

size_t SuccinctTree::get(Node node, size_t i, size_t j) const {
  // gets this quadrant out, just does the multiplication by
  // ROW_SIZE for the user
  return get(node, ROW_SIZE * i + j);
}

void SuccinctTree::set(Node node, size_t i, size_t j, size_t value) {
  ruler[node + (ROW_SIZE * i + j)] = value;
}

void SuccinctTree::set(Node node, size_t entry, size_t value) {
  ruler[node + entry] = value;
}

size_t SuccinctTree::get(Node node, size_t entry) const {
  // you have to give 0 <= entry <= 3
  // node number has to be less than length / 4
  // if you try to get something with 0 in it, we return 0
  if (ruler[node + entry] == EMPTY) return HEAD_NODE;

  if (!(node < ruler.size())) {
    std::cout << "Node: " << node << ", entry: " << entry << "\n";
  }
  assert(node < ruler.size());
  assert(entry >= 0 && entry < 4);

  return node + jumpSize(node + entry);
}

size_t SuccinctTree::getLength(Node node, size_t entry) const {
  return ruler[node + entry];
}

bool SuccinctTree::at(size_t i, size_t j) const {
  // deal with the case where the tree is empty
  if (ruler.empty()) return false;
  // base case is when dimension == 1 (a 1 by 1 matrix)?
  size_t dim = dimension;
  size_t quadrant;

  for (Node k = HEAD_NODE; dim > 1; k = get(k, quadrant), dim /= 2) {
    quadrant = quadrantNum(i, j, dim);
    if (ruler[k + quadrant] == EMPTY)
      return false;
    // don't we need to also adjust the quadrant variable?
    if (quadrant >= 2) i -= dim / 2;
    if (quadrant % 2 == 1) j -= dim / 2;
  }

  return true;
}

size_t SuccinctTree::jumpSize(size_t position) const {
  // find your next lowest multiple of 4
  size_t nodePos = position - position % NUM_CHILDREN;
  size_t jumpSize = NUM_CHILDREN;
  for (size_t i = nodePos; i < position; ++i) { jumpSize += ruler[i]; }
  return jumpSize;
}

size_t SuccinctTree::quadrantNum(size_t i, size_t j, size_t dim) const {
   int quadNum = 0;
   if (i >= dim / 2) quadNum += 2;
   if (j >= dim / 2) quadNum++;
   return quadNum;
}


/**
* I sort of want to go like okay I give a child
*
*/
size_t SuccinctTree::insert(std::string child) {
  // in the base case, we need to return 0
  if (child.find('1') == std::string::npos)
    return 0;

  // need to make new item at the end
  size_t index = ruler.size(); // will be zero the first time
  // this is WHERE The child is
  // how long is this child though?
  // She's the sum of the lengths of HER children
  size_t length = NUM_CHILDREN;
  // add 4 new entries to the back of this vector
  giveChild();
  // tail has [_ _ _ _]
  // with meaningful values in the loop below
  MatrixProcessor p;

  for (size_t i = 0; i < NUM_CHILDREN; ++i) {
    std::string grandChild = p.recoverQuadrant(child, i);
    size_t lenGran = insert(grandChild);
    ruler[index + i] = lenGran;
    length += lenGran;
  }
  return length;
}
