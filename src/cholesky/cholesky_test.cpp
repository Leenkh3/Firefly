#include "cholesky.hpp"
#include <Eigen/Dense>
#include <iostream>

// ✅ Returns true if test passes, false otherwise
bool testCholeskySolver() {
    // Define a symmetric positive definite matrix
    Eigen::MatrixXd A(3, 3);
    A << 4, 1, 2,
         1, 3, 0,
         2, 0, 5;

    Eigen::VectorXd b(3);
    b << 7, 4, 10;

    CholeskySolver solver;

    bool success = solver.compute(A);
    if (!success) {
        std::cerr << " Cholesky decomposition failed." << std::endl;
        return false;
    }

    Eigen::VectorXd x = solver.solve(b);
    Eigen::VectorXd Ax = A * x;

    if ((Ax - b).norm() >= 1e-6) {
        std::cerr << " Solution is incorrect. ||Ax - b|| = " << (Ax - b).norm() << std::endl;
        return false;
    }

    std::cout << " Dense Cholesky test passed!" << std::endl;
    std::cout << "Solution x:\n" << x << std::endl;
    return true;
}

int main() {
    return testCholeskySolver() ? 0 : 1;
}

