#include <cstddef>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "ConjugateGradient.hpp"

std::size_t shiftToZero(std::vector<std::size_t>& inpoel) {
    if (inpoel.empty()) return 0;
    auto minId = *std::min_element(begin(inpoel), end(inpoel));
    for (auto& n : inpoel) n -= minId;
    return minId;
}

int testLaplacian() {
    // Mesh connectivity for simple tetrahedron-only mesh (retained but unused)
    std::vector<std::size_t> inpoel {
        3, 13, 8, 14,
        12, 3, 13, 8,
        8, 3, 14, 11,
        12, 3, 8, 11,
        1, 2, 3, 13,
        6, 13, 7, 8,
        5, 9, 14, 11,
        5, 1, 3, 14,
        10, 4, 12, 11,
        2, 6, 12, 13,
        8, 7, 9, 14,
        13, 1, 7, 14,
        5, 3, 4, 11,
        6, 10, 12, 8,
        3, 2, 4, 12,
        10, 8, 9, 11,
        3, 1, 13, 14,
        13, 7, 8, 14,
        6, 12, 13, 8,
        9, 8, 14, 11,
        3, 5, 14, 11,
        4, 3, 12, 11,
        3, 2, 12, 13,
        10, 12, 8, 11
    };

    // Mesh node coordinates for simple tet mesh above (retained but unused)
    std::array<std::vector<double>, 3> coord {{
        {{-0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0}},
        {{0.5, 0.5, 0, -0.5, -0.5, 0.5, 0.5, 0, -0.5, -0.5, -0.5, 0, 0.5, 0}},
        {{-0.5, 0.5, 0, 0.5, -0.5, 0.5, -0.5, 0, -0.5, 0.5, 0, 0.5, 0, -0.5}}
    }};

    // Verification matrix (used as input matrix A)
    std::vector<std::vector<double>> dense_A = {
        {-0.5, -0.0416667, 0.208333, 0, -0.0416667, 0, -0.0416667, 0, 0, 0, 0, 0, 0.208333, 0.208333},
        {-0.0416667, -0.5, 0.208333, -0.0416667, 0, -0.0416667, 0, 0, 0, 0, 0, 0.208333, 0.208333, 0},
        {0.208333, 0.208333, -1.66667, 0.208333, 0.208333, 0, 0, -0.166667, 0, 0, 0.25, 0.25, 0.25, 0.25},
        {0, -0.0416667, 0.208333, -0.5, -0.0416667, 0, 0, 0, 0, -0.0416667, 0.208333, 0.208333, 0, 0},
        {-0.0416667, 0, 0.208333, -0.0416667, -0.5, 0, 0, 0, -0.0416667, 0, 0.208333, 0, 0, 0.208333},
        {0, -0.0416667, 0, 0, 0, -0.5, -0.0416667, 0.208333, 0, -0.0416667, 0, 0.208333, 0.208333, 0},
        {-0.0416667, 0, 0, 0, 0, -0.0416667, -0.5, 0.208333, -0.0416667, 0, 0, 0, 0.208333, 0.208333},
        {0, 0, -0.166667, 0, 0, 0.208333, 0.208333, -1.66667, 0.208333, 0.208333, 0.25, 0.25, 0.25, 0.25},
        {0, 0, 0, 0, -0.0416667, 0, -0.0416667, 0.208333, -0.5, -0.0416667, 0.208333, 0, 0, 0.208333},
        {0, 0, 0, -0.0416667, 0, -0.0416667, 0, 0.208333, -0.0416667, -0.5, 0.208333, 0.208333, 0, 0},
        {0, 0, 0.25, 0.208333, 0.208333, 0, 0, 0.25, 0.208333, 0.208333, -1.5, 0.0833333, 0, 0.0833333},
        {0, 0.208333, 0.25, 0.208333, 0, 0.208333, 0, 0.25, 0, 0.208333, 0.0833333, -1.5, 0.0833333, 0},
        {0.208333, 0.208333, 0.25, 0, 0, 0.208333, 0.208333, 0.25, 0, 0, 0, 0.0833333, -1.5, 0.0833333},
        {0.208333, 0, 0.25, 0, 0.208333, 0, 0.208333, 0.25, 0.208333, 0, 0.0833333, 0, 0.0833333, -1.5}
    };

    // Construct SparseCSR matrix from dense_A
    std::vector<int> cols;
    std::vector<int> rows_ptr = {0};
    std::vector<double> vals;
    int nnz = 0;
    for (std::size_t i = 0; i < dense_A.size(); ++i) {
        for (std::size_t j = 0; j < dense_A[i].size(); ++j) {
            if (dense_A[i][j] != 0.0) {
                cols.push_back(j + 1); // 1-based indexing
                vals.push_back(dense_A[i][j]);
                ++nnz;
            }
        }
        rows_ptr.push_back(nnz);
    }

    // Create SparseCSR matrix
    SparseCSR A(cols, rows_ptr, vals, dense_A.size(), dense_A[0].size());

    // Set up vectors
    std::vector<double> x(dense_A.size(), 0.0);
    std::vector<double> b(dense_A.size());
    std::vector<double> ones(dense_A.size(), 1.0);
    b = A.mult(ones); // b = A * ones for a consistent system

    // Solve using CG
    conjugateGradient(A, b, x);

    // Verify solution by computing residual norm ||b - A*x||
    std::vector<double> Ax = A.mult(x);
    double residual = 0.0;
    for (std::size_t i = 0; i < b.size(); ++i) {
        residual += (b[i] - Ax[i]) * (b[i] - Ax[i]);
    }
    residual = std::sqrt(residual);

    std::cout << "Residual norm: " << residual << std::endl;
    if (residual > 1e-6) {
        std::cerr << "CG solver failed: residual too large\n";
        return -1;
    }

    // Print solution
    std::cout << "Solution x: ";
    for (const auto& val : x) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}

int main(int argc, char* argv[]) {
    return testLaplacian();
}
