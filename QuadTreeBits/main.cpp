#include <iostream>
#include <fstream>
#include <sstream>
#include "node.h"
#include "quadtree.h"
#include "bitmult.h"

int main() {

    std::ifstream file1;
    std::ifstream file2;
    file1.open("../Matrix/matrix1.txt");
    file2.open("../Matrix/matrix2.txt");

    if (file1 && file2) {
       std::string matrix1;
       std::string matrix2;
       std::stringstream ss;
       std::stringstream ss2;
       ss << file1.rdbuf();
       ss2 << file2.rdbuf();
       matrix1 = ss.str();
       matrix2 = ss2.str();
    
   //  std::cout << "Node size: " << sizeof(Node) << ", Link size: " << sizeof(Link) << "\n";
       Quadtree q = Quadtree::Construct(matrix1);
       Quadtree p = Quadtree::Construct(matrix2);
       Quadtree product = q * p;
       std::cout << product << "\n";
     //  std::cout << Node::maxUsage * sizeof(Node)<< "\n";
    }

    file1.close();
    file2.close();
}
