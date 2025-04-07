#include "cholesky_CRS.hpp"
#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky> // Required for Eigen’s sparse Cholesky factorization


// Computes the Cholesky decomposition of a sparse matrix A.
// Returns true if the decomposition was successful.
bool CholeskySparseSolver::compute(const Eigen::SparseMatrix<double>& A) {
    solver.compute(A);  // Perform the Cholesky factorization
    return solver.info() == Eigen::Success;
}

// Solves the system Ax = b using the previously computed Cholesky factorization.
// Returns the solution vector x.

Eigen::VectorXd CholeskySparseSolver::solve(const Eigen::VectorXd& b)  {
    return solver.solve(b);
}
