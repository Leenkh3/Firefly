#include "SparseCSR.h"
#include <cassert>
#include <algorithm>

SparseCSR::SparseCSR(const std::vector<std::size_t>& connectivity, int shape_points) 
    : rows(shape_points), cols_count(shape_points) {
    rows_ptr = std::vector<int>(shape_points + 1, 0);
    cols = std::vector<int>();
    vals = std::vector<double>();
}

SparseCSR::SparseCSR(const std::vector<int>& cols_in, const std::vector<int>& rows_ptr_in, 
                     const std::vector<double>& vals_in, int rows_in, int cols_count_in)
    : cols(cols_in), rows_ptr(rows_ptr_in), vals(vals_in), rows(rows_in), cols_count(cols_count_in) {
    assert(rows_ptr.size() == static_cast<std::size_t>(rows) + 1);
    assert(cols.size() == vals.size());
}

double& SparseCSR::at(int row, int col) {
    for (int j = rows_ptr[row]; j < rows_ptr[row + 1]; ++j) {
        if (cols[j] == col + 1) {
            return vals[j];
        }
    }
    throw std::runtime_error("Element not found in SparseCSR");
}

std::vector<int> SparseCSR::shape() {
    return {rows, cols_count};
}

void SparseCSR::print() const {
    std::cout << "Rows: " << rows << ", Cols: " << cols_count << "\n";
    std::cout << "Row pointers: ";
    for (const auto& r : rows_ptr) std::cout << r << " ";
    std::cout << "\nColumns: ";
    for (const auto& c : cols) std::cout << c << " ";
    std::cout << "\nValues: ";
    for (const auto& v : vals) std::cout << v << " ";
    std::cout << "\n";
}

void SparseCSR::print_matrix() const {
    for (int i = 0; i < rows; ++i) {
        std::vector<double> row(cols_count, 0.0);
        for (int j = rows_ptr[i]; j < rows_ptr[i + 1]; ++j) {
            row[cols[j] - 1] = vals[j];
        }
        for (const auto& v : row) std::cout << v << "\t";
        std::cout << "\n";
    }
}

void SparseCSR::reshape(int rows_in, int cols_in) {
    rows = rows_in;
    cols_count = cols_in;
}

void SparseCSR::T() {
    throw std::runtime_error("Transpose not implemented");
}

std::vector<double> SparseCSR::mult(std::vector<double>& vec) const {
    assert(vec.size() == static_cast<std::size_t>(cols_count));
    std::vector<double> result(rows, 0.0);
    for (int i = 0; i < rows; ++i) {
        for (int j = rows_ptr[i]; j < rows_ptr[i + 1]; ++j) {
            result[i] += vals[j] * vec[cols[j] - 1];
        }
    }
    return result;
}

std::ostream& SparseCSR::write_matlab(std::ostream& os) const {
    os << "A = [ ";
    for (int i = 0; i < rows; ++i) {
        std::vector<double> row(cols_count, 0.0);
        for (int j = rows_ptr[i]; j < rows_ptr[i + 1]; ++j) {
            row[cols[j] - 1] = vals[j];
        }
        for (size_t j = 0; j < row.size(); ++j) {
            os << row[j];
            if (j < row.size() - 1) os << " ";
        }
        os << (i < rows - 1 ? ";\n" : "\n");
    }
    os << "];\n";
    return os;
}

bool operator==(std::vector<int>& a, std::vector<int>& b) {
    return a == b;
}
