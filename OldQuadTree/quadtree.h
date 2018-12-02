#ifndef QUADTREE_H
#define QUADTREE_H
#include <memory>
#include "matrixprocessor.h"
#include "node.h"
#include <string>
#include <cmath>
#include <iostream>

class Quadtree {
public:
    Quadtree(Node *head, size_t height);
    Quadtree(const Quadtree &other);
    /**
     * @brief construct is a class method
     * @param s - a string like [1001,1100, 0000, 0000] denotes a 4x4 matrix
     * @return a brand new node that the caller has ownership of
     */
    static Quadtree Construct(std::string text);

    bool at(size_t i, size_t j) const;

    /**
     * @brief operator <<
     * @param os
     * @param quadtree - the quadtree to represent in a string form
     * @return the modified stream object
     *
     * Writing this function is the only way I'll know
     * whether or not I actually processed a string and correctly
     * built a Quad tree. :O
     */
    friend std::ostream& operator<<(std::ostream& os, const Quadtree& quadtree);

    /**
     * @brief operator + takes the UNION between the two matrices
     * @param other
     * @return the union'd version (completely separate thing in memory)
     */
    Quadtree operator+(const Quadtree& other);

    Quadtree operator*(const Quadtree& other);

public:
    std::unique_ptr<Node> head;
    // the height of this quadtree
    size_t height;
};

#endif // QUADTREE_H
