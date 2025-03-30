#include "cholesky.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <cassert>

void testCholeskySolver() {
    // Test case 1: Simple positive definite matrix
    Eigen::MatrixXd A(3, 3);
    A << 4, 1, 2,
         1, 3, 0,
         2, 0, 5;

    Eigen::VectorXd b(3);
    b << 7, 4, 10;

    CholeskySolver solver;

    // Compute the Cholesky decomposition
    bool success = solver.compute(A);
    assert(success && "Cholesky decomposition failed.");

    // Solve for x in A * x = b
    Eigen::VectorXd x = solver.solve(b);


    std::cout << "Solution x:\n" << x << std::endl;

    // Verify that A * x == b
    Eigen::VectorXd Ax = A * x;
    assert((Ax - b).norm() < 1e-6 && "Solution does not satisfy A * x = b.");

    std::cout << "Test passed for simple positive definite matrix!" << std::endl;
}

int main() {
    // Run the test
    testCholeskySolver();
    
    return 0;
}

