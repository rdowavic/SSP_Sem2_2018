#include "node.h"
#include <cassert>
#include <iostream>

Link::Link() {
    new (&node) std::unique_ptr<Node>(nullptr);
}

Link::~Link() {}

// initialising static class variables
int Node::currentUsage = 0;
int Node::maxUsage = 0;

Node::Node() {}

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

    int newI = i, newJ = j;
    // find out which quadrant [i,j]-th entry is in and
    // adjust the i, j appropriately for the recursive call
    size_t halfMatrixSize = 1 << (distanceFromLeaf + 2);
    size_t quadrant = 0;

    if (i >= halfMatrixSize) {
        quadrant += 2;
        newI -= halfMatrixSize;
    }

    if (j >= halfMatrixSize) {
        quadrant++;
        newJ -= halfMatrixSize;
    }

    // do it here I reckon
    // may not be distance == 0, could be 1
    if (distanceFromLeaf == 1) {
        size_t bitPos = newI * ROW_LENGTH + newJ;
        uint64_t num = children[quadrant].leafMatrix;
        return (num >> (63 - bitPos)) & 1;
    }

    if (children[quadrant].node.get() == nullptr)
        return false;

    return children[quadrant].node
            ->at(newI, newJ, distanceFromLeaf - 1);
}

Node * Node::get(size_t i, size_t j) {
    Node * result = children[BLOCK_DIM * i + j].node.get();
    return result;
}

std::unique_ptr<Node> Node::Construct(std::string s) {
  // if everything in the string is a zero, return nullptr
  if (s.find("1") == std::string::npos)
      return std::unique_ptr<Node>(nullptr);

  auto result = std::make_unique<Node>();

  MatrixProcessor p;

  for (int i = 0; i < NUM_CHILDREN; ++i) {
      std::string quadrant = p.recoverQuadrant(s, i);
      // base case when the length is like that
      assert(s.length() >= 256);
      if (s.length() == 256) {
          result->children[i].leafMatrix = std::stoull(quadrant, 0, 2);
      } else {
         result->children[i].node = Construct(quadrant);
      }
  }

  return result;
}

std::unique_ptr<Node> Node::Clone(size_t distanceFromLeaf) {
    auto n = std::make_unique<Node>();

    if (distanceFromLeaf == 1) {
       for (int i = 0; i < NUM_CHILDREN; ++i)
           n->children[i].leafMatrix = children[i].leafMatrix;
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
    if (distanceFromLeaf == 1) {
        // directly union them
        for (int i = 0; i < NUM_CHILDREN; ++i) {
            result->children[i].leafMatrix =
                    A->children[i].leafMatrix | B->children[i].leafMatrix;
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
    if (distanceFromLeaf == 1) {
        // regular matrix multiplication
        // CHANGED CODE HERE
        //
        bool seenNonZero = false;
        for (int i = 0; i < BLOCK_DIM; ++i) {
            for (int j = 0; j < BLOCK_DIM; ++j) {
                uint64_t entry = 0;
                for (int k = 0; k < BLOCK_DIM; ++k) {
                    uint64_t first = A->children[BLOCK_DIM*i + k].leafMatrix;
                    uint64_t second = B->children[BLOCK_DIM*k + j].leafMatrix;
                    entry |= mult(first, second);
                }
                // entry has now been calculated
                result->children[BLOCK_DIM*i + j].leafMatrix = entry;
                if (entry != 0) seenNonZero = true;
            }
            // if all the entries are null, you need to return a nullptr
            // not a dumb thing yeah
            if (!seenNonZero) return std::unique_ptr<Node>(nullptr);
        }
        return result;
    }

    // we need to recursively compute the product here :) It should
    // be easier than last time
    for (int i = 0; i < BLOCK_DIM; ++i) {
        for (int j = 0; j < BLOCK_DIM; ++j) {
            // the quadrant, it's a node unique ptr
            auto n = std::unique_ptr<Node>(nullptr);

            for (int k = 0; k < BLOCK_DIM; ++k) {
                Node* first = A->get(i, k);
                Node* second = B->get(k, j);

                auto product = Product(first, second, distanceFromLeaf - 1);

                n = Union(n.get(), product.get(), distanceFromLeaf - 1);
            }
            result->children[BLOCK_DIM*i + j].node = std::move(n);
        }
    }
    return result;
}
