#include "SuccinctTree.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string getMatrix(const std::ifstream& file) {
  if (!file.is_open()) {
    std::cout << "File could not be opened\n";
    return "";
  }
  std::string matrix;
  std::stringstream ss;
  ss << file.rdbuf();
  matrix = ss.str();
  return matrix;
}

int main() {
  std::ifstream file1("../Matrix/matrix1.txt");
  std::ifstream file2("../Matrix/matrix2.txt");

  std::string m1 = getMatrix(file1);
  std::string m2 = getMatrix(file2);

  SuccinctTree tree1 = SuccinctTree::Construct(m1);
  SuccinctTree tree2 = SuccinctTree::Construct(m2);

  SuccinctTree result = tree1 * tree2;
  std::cout << result << "\n";

  file1.close(); file2.close(); return 0;
}


