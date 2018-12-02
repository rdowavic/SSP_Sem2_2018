#include "SuccinctTree.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream file1;
    std::ifstream file2;
    file1.open("../Matrix/matrix1.txt");
    file2.open("../Matrix/matrix2.txt");

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
       SuccinctTree q = SuccinctTree::Construct(matrix1);
       SuccinctTree p = SuccinctTree::Construct(matrix2);
       SuccinctTree product = q * p;
       std::cout << product << "\n";
     //  std::cout << Node::maxUsage * sizeof(Node)<< "\n";
    } else {
      std::cout << "Files could not be opened\n";  
    }

    file1.close();
    file2.close();
    return 0;

}
