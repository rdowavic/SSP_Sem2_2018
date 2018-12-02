#include <cassert>
#include "matrixprocessor.h"
#include <iostream>

MatrixProcessor::MatrixProcessor() {}

std::string MatrixProcessor::recoverQuadrant(std::string &s, int i) const {
    std::string result;
    /* I need to
     * My assumption is that the length of the string is a power of four
     * ie pow(4, something)
     * and something > 1, because if it's one then that's the base case
     * and I should not have even called this method because I should have
     * just made the Node* directly
     */
    int sizeLineQuadrant = sqrt(s.length()) / 2;
    // you need to take pieces of the string in half(size/4) chunks
    int startPosition =
            i <= 1 ?
                i * sizeLineQuadrant
              : ((i - 2) * sizeLineQuadrant) + s.length() / 2;
    int endPosition = (i <= 1) ? s.length() / 2 : s.length();

    for (int j = startPosition; j < endPosition; j += 2 * sizeLineQuadrant) {
        result += s.substr(j, sizeLineQuadrant);
    }

    return result;
}

std::string MatrixProcessor::strip(const std::string &s) const {
    std::string result;
    result.reserve(s.size());
    // only keeping 0s and 1s
    for (char c : s) {
        if (c == '0' || c == '1')
            result += c;
    }
    return result;
}
