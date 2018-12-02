#include <cassert>
#include "quadtree.h"

Quadtree::Quadtree(Node* head, size_t height) : head(head), height(height) {}
Quadtree::Quadtree(const Quadtree &other)
    : head(other.head->Clone(height - 1).release()),
      height(other.height) {}

bool Quadtree::at(size_t i, size_t j) const {
    if (head.get() == nullptr)
        return false;

    return head->at(i, j, height - 1);
}

std::ostream& operator<<(std::ostream& os,
                         const Quadtree& quadtree) {
//    size_t size = (1 << quadtree.height) * MIN_SIZE;
    size_t size = 1 << (quadtree.height + 2);

    os << "[";
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            bool val = quadtree.at(i, j);
            os << val;
        }
        if (i != size - 1) os << ", ";
    }
    os << "]";
    return os;
}

Quadtree Quadtree::operator+(const Quadtree& other) {
    return Quadtree(Node::Union(head.get(),
                                other.head.get(),
                                height - 1)
                    .release(), height);
}

Quadtree Quadtree::operator*(const Quadtree& other) {
    return Quadtree(Node::Product(head.get(),
                                  other.head.get(),
                                  height - 1)
                    .release(), height);
}

Quadtree Quadtree::Construct(std::string text) {
    MatrixProcessor processor;

    // CHECK THAT THE SIZE EVEN MAKES SENSE
    // FOR A QUADTREE
    // size 2 -> height 1
    // size 4 -> height 2

    std::string s = processor.strip(text);
    size_t size = (size_t) sqrt(s.length());
    assert(size * size == s.length());
    size_t height = (size_t) (log(size) / log(2)) - 2;
    // make sure this is right
    return Quadtree(Node::Construct(s).release(), height);
}
