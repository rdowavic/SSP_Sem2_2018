#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "node.h"
#include "quadtree.h"

int main() {

    std::ifstream file1("../Matrix/matrix1.txt");
    std::ifstream file2("../Matrix/matrix2.txt");

    if (file1.is_open() && file2.is_open()) {
       std::string matrix1;
       std::string matrix2;
       std::stringstream ss;
       std::stringstream ss2;
       ss << file1.rdbuf();
       ss2 << file2.rdbuf();
       matrix1 = ss.str();
       matrix2 = ss2.str();
    
   //  std::cout << "Node size: " << sizeof(Node) << ", Link size: " << sizeof(Link) << "\n";
       Quadtree m1 = Quadtree::Construct(matrix1);
       Quadtree m2 = Quadtree::Construct(matrix2);
       Quadtree product = m1 * m2;
       std::cout << product << "\n";
     //  std::cout << Node::maxUsage * sizeof(Node)<< "\n";
    }

    file1.close();
    file2.close();

}
