#include "cholesky.hpp"
#include <iostream>
#include <Eigen/Dense>  // Include Eigen's dense matrix support

// Perform Cholesky decomposition of a dense matrix A (A = L * Lᵗ)

bool CholeskySolver::compute(const Eigen::MatrixXd& A) {
    // Directly compute the Cholesky decomposition without checking
    Eigen::LLT<Eigen::MatrixXd> llt(A); // LLT = Cholesky for symmetric positive-definite matrices
    L = llt.matrixL();  // Store the lower triangular matrix L
    return true;  // Return true as the decomposition will always succeed here
}

// Solve the linear system A * x = b using the computed L
Eigen::VectorXd CholeskySolver::solve(const Eigen::VectorXd& b) const {
    // First solve L * y = b for y using forward substitution
    // Then solve Lᵗ * x = y for x using backward substitution
    return L.transpose().triangularView<Eigen::Upper>().solve(
           L.triangularView<Eigen::Lower>().solve(b));
}


