#ifndef CHOLESKY_HPP
#define CHOLESKY_HPP

#include <Eigen/Dense>

//  A simple wrapper around Eigen's LLT Cholesky decomposition
//    for solving linear systems with dense, symmetric positive definite matrices.

class CholeskySolver {
public:
    // Constructor and destructor
    CholeskySolver() = default;
    ~CholeskySolver() = default;

      // Computes the Cholesky decomposition (A = L * Lᵀ)
    // A must be symmetric and positive definite
    bool compute(const Eigen::MatrixXd& A);

      //  Solves the system A * x = b using the previously computed decomposition
    //    Returns the solution vector x
    Eigen::VectorXd solve(const Eigen::VectorXd& b) const;

private:
    // Stores the lower triangular matrix L from A = L * Lᵀ
    Eigen::MatrixXd L;
};

#endif // CHOLESKY_HPP
