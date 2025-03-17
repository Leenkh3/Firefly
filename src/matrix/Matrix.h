#pragma once

#include <vector>

class Matrix
{
public:
    virtual std::vector<int> shape() = 0;                           // Get matrix dimensions
    virtual void reshape(int rows, int cols) = 0;           // Reshape matrix
    virtual void T() = 0;                                   // Transpose matrix
    virtual void dot(std::vector<double> &vec) = 0;              // Dot product with vector
    virtual void dot(std::vector<std::vector<double> > &mat) = 0;     // Dot product with matrix
    virtual void print() = 0;                                  // Print matrix
    virtual double at(int row, int col) = 0;                    // get the value at row,col
    virtual ~Matrix() {}                                       // Virtual destructor
};


