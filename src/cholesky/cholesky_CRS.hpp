#pragma once
#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>

class CholeskySparseSolver {
public:
    // Compute the Cholesky decomposition of a sparse matrix
    bool compute(const Eigen::SparseMatrix<double>& A);

    // Solve the linear system A * x = b
    Eigen::VectorXd solve(const Eigen::VectorXd& b) const;

private:
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
};
