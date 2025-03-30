#include "cholesky_CRS.hpp"
#include <iostream>
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>

bool CholeskySparseSolver::compute(const Eigen::SparseMatrix<double>& A) {
    solver.compute(A);  // Perform the Cholesky factorization
    return solver.info() == Eigen::Success;
}

Eigen::VectorXd CholeskySparseSolver::solve(const Eigen::VectorXd& b)  {
    return solver.solve(b);
}
