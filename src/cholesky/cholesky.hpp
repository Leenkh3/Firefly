#ifndef CHOLESKY_HPP
#define CHOLESKY_HPP

#include <Eigen/Dense>

class CholeskySolver {
public:
    // Constructor and destructor
    CholeskySolver() = default;
    ~CholeskySolver() = default;

    // Function to compute the Cholesky decomposition of matrix A
    bool compute(const Eigen::MatrixXd& A);

    // Function to solve the system A * x = b using the computed Cholesky decomposition
    Eigen::VectorXd solve(const Eigen::VectorXd& b) const;

private:
    // Lower triangular matrix from the Cholesky decomposition
    Eigen::MatrixXd L;
};

#endif // CHOLESKY_HPP