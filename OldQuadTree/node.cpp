#include "node.h"
#include <iostream>

Link::Link() {
    new (&node) std::unique_ptr<Node>(nullptr);
}

Link::~Link() {}

Node::Node() {}

int Node::currentUsage = 0;
int Node::maxUsage = 0;

void* operator new(size_t size) noexcept {
    // the user made a new node so pls record it
    Node::currentUsage++;
    // do a max check
    if (Node::currentUsage > Node::maxUsage)
	Node::maxUsage = Node::currentUsage;
    
    void* p = std::malloc(size);
    return p;
}

void operator delete(void* p) noexcept {
    Node::currentUsage--;
    std::free(p);
}

bool Node::at(size_t i, size_t j, size_t distanceFromLeaf) {

    if (distanceFromLeaf == 0) {
        // then we are ourselves a leaf! return the thing
        return children[MIN_SIZE*i + j].value;
    }

    int newI = i, newJ = j;
    // find out which quadrant [i,j]-th entry is in and
    // adjust the i, j appropriately for the recursive call

    /**
     * @brief halfMatrixSize given the distance is 5,
     * that means the height is 6, that means the size is 64.
     */
    size_t halfMatrixSize = 1 << distanceFromLeaf;
    size_t quadrant = 0;

    if (i >= halfMatrixSize) {
        quadrant += MIN_SIZE;
        newI -= halfMatrixSize;
    }

    if (j >= halfMatrixSize) {
        quadrant++;
        newJ -= halfMatrixSize;
    }

    if (children[quadrant].node.get() == nullptr)
        return false;

    return children[quadrant].node
            ->at(newI, newJ, distanceFromLeaf - 1);
}

Node * Node::get(size_t i, size_t j) {
    Node * result = children[MIN_SIZE * i + j].node.get();
    return result;
}

std::unique_ptr<Node> Node::Construct(std::string s) {
  // if everything in the string is a zero, return nullptr
  if (s.find("1") == std::string::npos)
      return std::unique_ptr<Node>(nullptr);

  auto result = std::make_unique<Node>();

  // base case
  if (s.length() == NUM_CHILDREN) {
      for (int i = 0; i < NUM_CHILDREN; ++i)
          result->children[i].value = s.at(i) == '1';
      return result;
  }

  MatrixProcessor p;

  for (int i = 0; i < NUM_CHILDREN; ++i) {
      std::string quadrant = p.recoverQuadrant(s, i);
      result->children[i].node = Construct(quadrant);
  }

  return result;
}

std::unique_ptr<Node> Node::Clone(size_t distanceFromLeaf) {
    auto n = std::make_unique<Node>();

    if (distanceFromLeaf == 0) {
       for (int i = 0; i < NUM_CHILDREN; ++i)
           n->children[i].value = children[i].value;
       return n;
    }

    for (int i = 0; i < NUM_CHILDREN; ++i) {
        n->children[i].node =
                children[i].node.get() == nullptr ?
                   std::unique_ptr<Node>(nullptr) :
                   children[i].node->Clone(distanceFromLeaf - 1);
    }

    return n;
}

std::unique_ptr<Node> Node::Union
    (Node * const A, Node * const B, size_t distanceFromLeaf) {

    // if only one of them is nullptr, give back the other one
    if (A == nullptr ^ B == nullptr) {
        return (A == nullptr) ?
                    B->Clone(distanceFromLeaf) :
                    A->Clone(distanceFromLeaf);
    }
    // if both are nullptr, give back unique ptr to nullptr
    if (A == nullptr && B == nullptr)
        return std::unique_ptr<Node>(nullptr);

    // now both are not nullptr
    auto result = std::make_unique<Node>();
    // deal with base case
    if (distanceFromLeaf == 0) {
        // directly union them
        for (int i = 0; i < NUM_CHILDREN; ++i) {
            result->children[i].value =
                    A->children[i].value || B->children[i].value;
        }
    } else {
        //deal with recursive case, easy! just construct a new node and each of its children
        // nodes are the union of blahs
        for (int i = 0; i < NUM_CHILDREN; ++i) {
            result->children[i].node =
                    Union(A->children[i].node.get(),
                          B->children[i].node.get(),
                          distanceFromLeaf - 1);
        }
    }
    return result;
}

std::unique_ptr<Node> Node::Product
    (Node * const A, Node * const B, size_t distanceFromLeaf) {

    /*
     * In a product, if either side is 'all zeros', the result is always
     * all zeros.
     */

    //1. Check out whether either are nullptr, then return nullptr
    if (A == nullptr || B == nullptr) {
        return std::unique_ptr<Node>(nullptr);
    }

    // Neither are nullptr now

    auto result = std::make_unique<Node>();
    //2. Check for base case
    if (distanceFromLeaf == 0) {
        // regular matrix multiplication
        for (int i = 0; i < MIN_SIZE; ++i) {
            for (int j = 0; j < MIN_SIZE; ++j) {
                bool entry = false;
                for (int k = 0; k < MIN_SIZE; ++k) {
                    entry = entry || (A->at(i, k, 0) && B->at(k, j, 0));
                }
                // entry has now been calculated
                result->children[MIN_SIZE*i + j].value = entry;
            }
        }
        return result;
    }

    // we need to recursively compute the product here :) It should
    // be easier than last time
    for (int i = 0; i < MIN_SIZE; ++i) {
        for (int j = 0; j < MIN_SIZE; ++j) {
            // the quadrant, it's a node unique ptr
            auto n = std::unique_ptr<Node>(nullptr);

            for (int k = 0; k < MIN_SIZE; ++k) {
                Node* first = A->get(i, k);
                Node* second = B->get(k, j);

                auto product = Product(first, second, distanceFromLeaf - 1);

                n = Union(n.get(), product.get(), distanceFromLeaf - 1);
            }
            result->children[MIN_SIZE*i + j].node = std::move(n);
        }
    }
    return result;
}
