#include "utils.h"
#include <iostream>

void testIDMatrix(size_t dimension) {
  // just call the util function from here
  // 1. this makes the idMatrix come into the file
  std::stringstream s;
  s << "python3 ../idMatrix.py " << dimension << " > result.txt";
  system(s.str().c_str());
  // 2. get that string
  std::string contents = getFileContents("result.txt");
  SuccinctTree t = SuccinctTree::Construct(contents);
  // 3. compare it to what the SuccinctTree says it is
  MatrixProcessor processor;
  std::string shouldBe = processor.prettify(contents);
  std::stringstream ss; ss << t;
  std::string actuallyIs = ss.str();

  assert(shouldBe == actuallyIs);
}

void testRandomMatrix(size_t dimension) {
  std::stringstream s;
  s << "python3 ../randomSparseMatrix.py " << dimension << " > result.txt";
  system(s.str().c_str());
  // 2. get that string
  std::string contents = getFileContents("result.txt");
  SuccinctTree t = SuccinctTree::Construct(contents);
  // 3. compare it to what the SuccinctTree says it is
  MatrixProcessor processor;
  std::string shouldBe = processor.prettify(contents);
  std::stringstream ss; ss << t;
  std::string actuallyIs = ss.str();

  assert(shouldBe == actuallyIs);
}

void testRandomProductWithID(size_t dimension) {
  std::stringstream s;
  s << "python3 ../randomSparseMatrix.py " << dimension << " > result.txt";
  system(s.str().c_str());
  // 2. get that string
  std::string contents = getFileContents("result.txt");
  SuccinctTree t = SuccinctTree::Construct(contents);
  // make one for product



  // 3. compare it to what the SuccinctTree says it is
  MatrixProcessor processor;
  std::string shouldBe = processor.prettify(contents);
  std::stringstream ss; ss << t;
  std::string actuallyIs = ss.str();

  assert(shouldBe == actuallyIs);
}

std::string getFileContents(std::string path) {
  std::ifstream file;
  std::string contents;
  file.open(path);
  if (file) {
    std::stringstream ss;
    ss << file.rdbuf();
    contents = ss.str();
  }
  file.close();
  return contents;
}
