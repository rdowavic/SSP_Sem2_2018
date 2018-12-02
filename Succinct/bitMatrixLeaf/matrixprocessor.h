#ifndef MATRIXPROCESSOR_H
#define MATRIXPROCESSOR_H
#include <string>
#include <cmath>

class MatrixProcessor
{
public:
    MatrixProcessor();
    std::string recoverQuadrant(std::string& s, int i) const;
    std::string strip(const std::string& s) const;
    std::string prettify(const std::string& s) const;
};

#endif // MATRIXPROCESSOR_H
