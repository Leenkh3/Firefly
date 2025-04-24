#pragma once
#include "Matrix.h"
#include <iostream>
#include <vector>

class SparseCSR : public Matrix {
private:
    std::vector<int> cols;
    std::vector<int> rows_ptr;
    std::vector<double> vals;
    int rows, cols_count; // Store dimensions

public:
    SparseCSR(const std::vector<std::size_t>& connectivity, int shape_points);
    SparseCSR(const std::vector<int>& cols_in, const std::vector<int>& rows_ptr_in, const std::vector<double>& vals_in, int rows_in, int cols_count_in);
    double& at(int row, int col) override;
    std::vector<int> shape() override;
    void print() const override;
    void print_matrix() const override; // Matches Matrix.h
    void reshape(int rows, int cols) override;
    void T() override;
    const std::vector<int>& getRPtr() { return rows_ptr; }
    const std::vector<int>& getCols() { return cols; }
    const std::vector<double>& getVals() { return vals; }
    std::vector<double> mult(std::vector<double>& vec) const override;
    std::ostream& write_matlab(std::ostream& os) const;
    friend bool operator==(std::vector<int>& a, std::vector<int>& b);
};
