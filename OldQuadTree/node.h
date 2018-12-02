#ifndef NODE_H
#define NODE_H
#include <array>
#include <memory>
#include <string>
#include "matrixprocessor.h"

const int NUM_CHILDREN = 4;
const int MIN_SIZE = 2;

/**
 * So the Link constructor and destructor got implicitly
 * deleted because they weren't happy that the
 * Node class has a non-trivial default constructor.
 * I get that it's not trivial because it involves initialising that array
 *
 * but I am not sure how or if it's possible to get the variant
 * field 'node' to still have a non-trivial default constructor and have this still work
 *
 *
 * I remember them mentioning something about having to implement all the stuff for yourself
 * and then it will be fine
 *
 * If a union contains a non-static data member with a non-trivial
 * special member function (copy/move constructor, copy/move assignment,
 * or destructor), that function is deleted by default in the union and needs
 * to be defined explicitly by the programmer.
 * If a union contains a non-static data member with a non-trivial default constructor,
 * the default constructor of the union is deleted by default unless a variant member
 * of the union has a default member initializer .
 *
 *
 */
class Node;

typedef union Link {
    Link();
    ~Link();

    bool value;
    std::unique_ptr<Node> node;
} Link;

class Node {
public:
    Node();
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


    bool at(size_t i, size_t j, size_t distanceFromLeaf);
    // this method fetches a node child, NOT a boolean value
    Node *get(size_t i, size_t j);
    static std::unique_ptr<Node> Construct(std::string s);
    std::unique_ptr<Node> Clone(size_t distanceFromLeaf);
    // methods of traversal! make it now
    static std::unique_ptr<Node> Union(Node* const A, Node* const B, size_t distanceFromLeaf);
    static std::unique_ptr<Node> Product(Node* const A, Node* const B, size_t distanceFromLeaf);

//members
public:
    std::array<Link, NUM_CHILDREN> children;
    static int currentUsage;
    static int maxUsage;
};

#endif // NODE_H
