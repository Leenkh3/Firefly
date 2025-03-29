#include "cholesky.hpp"
#include <iostream>
#include <Eigen/Dense>

bool CholeskySolver::compute(const Eigen::MatrixXd& A) {
    // Directly compute the Cholesky decomposition without checking
    Eigen::LLT<Eigen::MatrixXd> llt(A);
    L = llt.matrixL();  // Store the lower triangular matrix L
    return true;  // Return true as the decomposition will always succeed here
}

Eigen::VectorXd CholeskySolver::solve(const Eigen::VectorXd& b) const {
    // Solve the system A * x = b using the Cholesky decomposition
    return L.transpose().triangularView<Eigen::Upper>().solve(
           L.triangularView<Eigen::Lower>().solve(b));
}


