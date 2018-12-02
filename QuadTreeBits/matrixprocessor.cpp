#include "matrixprocessor.h"

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

    // now my situation is that I still need to get this thing out

    int sizeLineQuadrant = sqrt(s.length()) / 2;
//    // you need to take pieces of the string in half(size/4) chunks
//    int startPosition = sizeLineQuadrant * (i <= 1 ? i : i + 2);
    int startPosition = sizeLineQuadrant
            * ( i <= 1 ?
                    i :
                    sqrt(s.length()) + (i - 2));

//  int startPosition = sizeLineQuadrant * i * (i <= 1 ? 1 : sqrt(s.length()));
    int endPosition = (i <= 1) ? s.length() / 2 : s.length();
    result.reserve(endPosition - startPosition);

    for (int i = startPosition; i < endPosition; i += 2 * sizeLineQuadrant) {
        result += s.substr(i, sizeLineQuadrant);
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
