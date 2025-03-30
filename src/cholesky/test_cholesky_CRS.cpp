#include "cholesky_sparse_solver.hpp"
#include <Eigen/Sparse>
#include <iostream>
#include <cassert>

void testCholeskySparseSolver() {
    // Define a 3x3 symmetric positive definite sparse matrix in CRS style
    Eigen::SparseMatrix<double> A(3, 3);
    std::vector<Eigen::Triplet<double>> triplets;

    // Fill the matrix in symmetric CRS format
    triplets.emplace_back(0, 0, 4);
    triplets.emplace_back(0, 1, 1);
    triplets.emplace_back(0, 2, 2);
    triplets.emplace_back(1, 1, 3);
    triplets.emplace_back(2, 2, 5);

    // Set the values into A
    A.setFromTriplets(triplets.begin(), triplets.end());

    // Make sure the matrix is symmetric
    A = A.selfadjointView<Eigen::Lower>();

    // Right-hand side vector
    Eigen::VectorXd b(3);
    b << 7, 4, 10;

    // Create and test the solver
    CholeskySparseSolver solver;
    bool success = solver.compute(A);
    assert(success && "Sparse Cholesky decomposition failed.");

    Eigen::VectorXd x = solver.solve(b);

    // Check that A * x ≈ b
    Eigen::VectorXd Ax = A * x;
    assert((Ax - b).norm() < 1e-6 && "Solution does not satisfy A * x = b.");

    std::cout << "Test passed for sparse Cholesky (CRS) matrix!" << std::endl;
}

int main() {
    testCholeskySparseSolver();
    return 0;
}
