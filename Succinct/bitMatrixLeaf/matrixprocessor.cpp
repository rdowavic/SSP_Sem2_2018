#include "matrixprocessor.h"

MatrixProcessor::MatrixProcessor() {}

std::string MatrixProcessor::recoverQuadrant(std::string &s, int i) const {
    if (s.length() <= 1) return "";

    std::string result;
    int sizeLineQuadrant = sqrt(s.length()) / 2;
    int startPos;
    if (i <= 1)
        startPos = sizeLineQuadrant * i;
    else
        // you have to get half way down the string
        startPos = s.length() / 2 + sizeLineQuadrant * (i - 2);

    int endPos = (i <= 1) ? s.length() / 2 : s.length();
    for (int i = startPos; i < endPos; i += 2 * sizeLineQuadrant) {
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

std::string MatrixProcessor::prettify(const std::string& s) const {
  std::string stripped = strip(s);
  std::string result = "[";
  // figure out the row length
  size_t dimension = (size_t) sqrt(stripped.length());

  for (size_t i = 0; i < stripped.length(); ++i) {
    if (i != 0 && i % dimension == 0)
      result += ", ";
    result += stripped[i];
  }
  result += "]";
  return result;
}
