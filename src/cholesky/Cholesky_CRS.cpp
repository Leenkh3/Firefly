#include "cholesky_sparse_solver.hpp"
#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>

bool CholeskySparseSolver::compute(const Eigen::SparseMatrix<double>& A) {
    solver.compute(A);  // Perform the Cholesky factorization
    return solver.info() == Eigen::Success;
}

Eigen::VectorXd CholeskySparseSolver::solve(const Eigen::VectorXd& b) const {
    return solver.solve(b);
}
