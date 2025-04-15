#include "cholesky.hpp" // Include the Cholesky solver class
#include <Eigen/Dense>
#include <iostream>

// Returns true if test passes, false otherwise
bool testCholeskySolver() {
    // Define a symmetric positive definite matrix
    Eigen::MatrixXd A(3, 3);
    A << 4, 1, 2,
         1, 3, 0,
         2, 0, 5;
// Define the right-hand side vector b
    Eigen::VectorXd b(3);
    b << 7, 4, 10;
// Create an instance of the CholeskySolver
    CholeskySolver solver;
// Compute the Cholesky factorization of A
    bool success = solver.compute(A);
    if (!success) {
        std::cerr << " Cholesky decomposition failed." << std::endl;
        return false;
    }
// Solve the system Ax = b using the Cholesky decomposition
    Eigen::VectorXd x = solver.solve(b);
 // Compute Ax to validate the result
    Eigen::VectorXd Ax = A * x;
 // Check that Ax ≈ b by comparing the norm of the residual
    if ((Ax - b).norm() >= 1e-6) {
        std::cerr << " Solution is incorrect. ||Ax - b|| = " << (Ax - b).norm() << std::endl;
        return false;
    }
// If everything is correct, print success and solution
    std::cout << " Dense Cholesky test passed!" << std::endl;
    std::cout << "Solution x:\n" << x << std::endl;
    return true;
}

int main() {
    return testCholeskySolver() ? 0 : 1;
}

