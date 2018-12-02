#include <iostream>
#include <cassert>
#include "utils.h"

int main() {
  // run tests
  // does it construct ID matrices properly?
  for (size_t i = 1; i < 10; ++i) {
    size_t dim = 1 << i;
    std::cout
        << "ID Matrix: "
        << dim << " x " << dim;
    // this has SO many side effects
    testIDMatrix(dim);
    std::cout << " OK!\n";
  }
  // does it construct randomly generated matrices properly?
  for (size_t i = 1; i < 10; ++i) {
    size_t dim = 1 << i;
    std::cout << "Random " << dim << " x " << dim << ": ";
    testRandomMatrix(dim);
    std::cout << "OK!\n";
  }
  // does it compute products properly?
  return 0;
}
