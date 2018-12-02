#ifndef BITMULT_H
#define BITMULT_H

#include <cstdint>
#include <stddef.h>

/**
This has all the utility functions for recovering a particular
row or column from the matrix, and multiplying one 8x8 bit matrix by another!
**/
uint8_t row(uint64_t n, size_t k);
uint8_t col(uint64_t n, size_t k);
uint64_t mult(uint64_t A, uint64_t B);

#endif // BITMULT_H
