#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "../Succinct/matrixprocessor.h"

int main(int argc, char** argv) {
  
  if (argc < 1) 
    std::cout << "Not enough arguments, bye\n";

  // 1. get file path from argv
  std::ifstream file(argv[1]); 
  // 2. load it into a string buffer
  std::stringstream buffer; buffer << file.rdbuf();
  std::string matrix = buffer.str();
  // 3. prettify the string using matrixprocessor
  MatrixProcessor p;
  std::string pretty = p.prettify(matrix);
  // 4. write out the string to disk, to the same destination (product.txt)
  file.close();
  // open the output file
  std::ofstream file2(argv[1]);
  // writing the string to disk
  file2 << pretty << "\n";
    
  file2.close();

  return 0;
}
