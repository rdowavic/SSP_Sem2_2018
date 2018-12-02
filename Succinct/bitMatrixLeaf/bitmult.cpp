#include "bitmult.h"


uint8_t row(uint64_t n, size_t k) {
    return (n >> ((7 - k) * 8)) & 255;
}

uint8_t col(uint64_t n, size_t k) {
    return ((n >> (56 - k)) & 128)
     | ((n >> (49 - k)) & 64)
     | ((n >> (42 - k)) & 32)
     | ((n >> (35 - k)) & 16)
     | ((n >> (28 - k)) & 8)
     | ((n >> (21 - k)) & 4)
     | ((n >> (14 - k)) & 2)
     | ((n >> (7 - k)) & 1);
}

uint64_t mult(uint64_t A, uint64_t B) {
    uint64_t result = 0;
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        uint8_t pos = 63 - (8*i + j);
        uint8_t product = row(A, i) & col(B, j);
        result |= (((uint64_t) (product != 0)) << pos);
      }
    }
   return result;
}

