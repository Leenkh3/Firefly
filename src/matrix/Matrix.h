#pragma once

#include <vector>

class Matrix
{
public:
    virtual std::vector<int> shape() = 0;                           // Get matrix dimensions
    virtual void reshape(int rows, int cols) = 0;           // Reshape matrix
    virtual void T() = 0;                                   // Transpose matrix
    virtual std::vector<double> mult(std::vector<double> &x) const = 0;              // Multiplication with vector
    virtual void print() const = 0;                                  // Print matrix
    virtual double& at(int row, int col) = 0;                    // get the value at row,col
    virtual ~Matrix() {}                                       // Virtual destructor
};


