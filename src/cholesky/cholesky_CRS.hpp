#ifndef CHOLESKY_CRS_HPP
#define CHOLESKY_CRS_HPP

#include <Eigen/Sparse>


//Class for performing Cholesky factorization on sparse matrices
// using Eigen's SimplicialLLT decomposition
class CholeskySparseSolver {
public:
// Perform the Cholesky decomposition of matrix A
    // A must be symmetric and positive definite
    bool compute(const Eigen::SparseMatrix<double>& A);
    Eigen::VectorXd solve(const Eigen::VectorXd& b);

private:
// Eigen's built-in sparse Cholesky solver object
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
};

#endif

