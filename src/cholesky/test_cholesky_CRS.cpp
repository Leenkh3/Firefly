#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>
#include <iostream>
#include <cassert>

// Assume your CholeskySparseSolver is just a wrapper for Eigen’s SimplicialLLT
struct CholeskySparseSolver {
    Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver;

    bool compute(const Eigen::SparseMatrix<double>& A) {
        solver.compute(A);
        return solver.info() == Eigen::Success;
    }

    Eigen::VectorXd solve(const Eigen::VectorXd& b) {
        return solver.solve(b);
    }
};






void testCholeskySparseSolver() {
    Eigen::SparseMatrix<double> A(3, 3);
    std::vector<Eigen::Triplet<double>> triplets;

    // Fill only lower triangle (for selfadjointView)
    triplets.emplace_back(0, 0, 4);
    triplets.emplace_back(1, 0, 1);
    triplets.emplace_back(2, 0, 2);
    triplets.emplace_back(1, 1, 3);
    triplets.emplace_back(2, 2, 5);

    A.setFromTriplets(triplets.begin(), triplets.end());
    A.makeCompressed();

    Eigen::VectorXd b(3);
    b << 7, 4, 10;

    CholeskySparseSolver solver;
    bool success = solver.compute(A.selfadjointView<Eigen::Lower>());
    assert(success && "Sparse Cholesky decomposition failed.");

    Eigen::VectorXd x = solver.solve(b);

    // This line changed:
    Eigen::VectorXd Ax = A.selfadjointView<Eigen::Lower>() * x;
    assert((Ax - b).norm() < 1e-6 && "Solution does not satisfy A * x = b.");

    std::cout << "✅ Sparse Cholesky test passed!" << std::endl;
    std::cout << "Solution x:\n" << x << std::endl;
}





int main() {
    testCholeskySparseSolver();
    return 0;
}


