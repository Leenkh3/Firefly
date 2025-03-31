#include <Eigen/Sparse>
#include <Eigen/SparseCholesky>
#include <iostream>
#include <vector>

// Wrapper for Eigen’s SimplicialLLT
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

// ✅ Returns true if test passes, false if fails
bool testCholeskySparseSolver() {
    Eigen::SparseMatrix<double> A(3, 3);
    std::vector<Eigen::Triplet<double>> triplets;

    // Lower triangle of SPD matrix
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
    if (!solver.compute(A.selfadjointView<Eigen::Lower>())) {
        std::cerr << " Sparse Cholesky decomposition failed!" << std::endl;
        return false;
    }

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
    return testCholeskySparseSolver() ? 0 : 1;
}

