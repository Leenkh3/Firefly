#pragma once

#include <vector>

class Matrix {
public:
    virtual std::vector<int> shape() = 0;
    virtual void reshape(int rows, int cols) = 0;
    virtual void T() = 0;
    virtual std::vector<double> mult(std::vector<double>& x) const = 0;
    virtual void print() const = 0;
    virtual void print_matrix() const = 0; // Added for SparseCSR
    virtual double& at(int row, int col) = 0;
    virtual ~Matrix() {}
};
