#ifndef CHOLESKY_CRS_HPP
#define CHOLESKY_CRS_HPP

#include <Eigen/Sparse>

class CholeskySparseSolver {
public:
    bool compute(const Eigen::SparseMatrix<double>& A);
    Eigen::VectorXd solve(const Eigen::VectorXd& b);

private:
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
};

#endif

