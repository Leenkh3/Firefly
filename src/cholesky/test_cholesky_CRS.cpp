#include <Eigen/Sparse> // For sparse matrix types
#include <Eigen/SparseCholesky> // For SimplicialLLT Cholesky solver
#include <iostream>
#include <vector>

// Wrapper for Eigen’s SimplicialLLT
struct CholeskySparseSolver {
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;
// Perform the Cholesky factorization
    bool compute(const Eigen::SparseMatrix<double>& A) {
        solver.compute(A);
        return solver.info() == Eigen::Success;
    }
 // Solve Ax = b using the computed Cholesky factorization
    Eigen::VectorXd solve(const Eigen::VectorXd& b) {
        return solver.solve(b);
    }
};

// Returns true if test passes, false if fails
bool testCholeskySparseSolver() {
 // Step 1: Create a 3x3 sparse matrix
    Eigen::SparseMatrix<double> A(3, 3);
    std::vector<Eigen::Triplet<double>> triplets;

  // Fill only the lower triangle of the symmetric positive definite matrix
    triplets.emplace_back(0, 0, 4);
    triplets.emplace_back(1, 0, 1);
    triplets.emplace_back(2, 0, 2);
    triplets.emplace_back(1, 1, 3);
    triplets.emplace_back(2, 2, 5);
// Step 2: Build the sparse matrix from triplets

    A.setFromTriplets(triplets.begin(), triplets.end());
    A.makeCompressed();
// Step 3: Define the right-hand side vector b
    Eigen::VectorXd b(3);
    b << 7, 4, 10;
// Step 4: Instantiate solver and compute the decomposition
    CholeskySparseSolver solver;
    if (!solver.compute(A.selfadjointView<Eigen::Lower>())) {
        std::cerr << " Sparse Cholesky decomposition failed!" << std::endl;
        return false;
    }
 // Step 5: Solve the system and check the result

    Eigen::VectorXd x = solver.solve(b);
    Eigen::VectorXd Ax = A.selfadjointView<Eigen::Lower>() * x;

    if ((Ax - b).norm() >= 1e-6) {
        std::cerr << " Solution is incorrect. ||Ax - b|| = " << (Ax - b).norm() << std::endl;
        return false;
    }

    std::cout << " Sparse Cholesky test passed!" << std::endl;
    std::cout << "Solution x:\n" << x << std::endl;
    return true;
}

int main() {
 // Run the sparse Cholesky test
    return testCholeskySparseSolver() ? 0 : 1;
}

