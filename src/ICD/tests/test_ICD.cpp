#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "ICD.h"

using namespace std;

// Helper function to compare matrices
bool matricesAlmostEqual(const vector<vector<double>>& A, const vector<vector<double>>& B, double tol = 1e-6) {
    if (A.size() != B.size()) return false;
    for (size_t i = 0; i < A.size(); i++) {
        if (A[i].size() != B[i].size()) return false;
        for (size_t j = 0; j < A[i].size(); j++) {
            if (fabs(A[i][j] - B[i][j]) > tol) {
                return false;
            }
        }
    }
    return true;
}

// Test ICD on a dense SPD matrix
bool testDenseMatrix() {
    vector<vector<double>> A = {
        {4.0, 1.0, 1.0},
        {1.0, 3.0, 0.5},
        {1.0, 0.5, 2.0}
    };

    vector<vector<double>> L = incompleteCholesky(A);

    // Check if L is lower triangular
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (L[i][j] != 0.0) {
                cerr << "L is not lower triangular!" << endl;
                return false;
            }
        }
    }

    // Reconstruct A ≈ L*Lᵀ
    vector<vector<double>> A_reconstructed(3, vector<double>(3, 0.0));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k <= min(i, j); k++) {
                A_reconstructed[i][j] += L[i][k] * L[j][k];
            }
        }
    }

    if (!matricesAlmostEqual(A, A_reconstructed)) {
        cerr << "ICD reconstruction failed for dense matrix!" << endl;
        return false;
    }
    return true;
}

// Test ICD on a sparse matrix (preserve zeros)
bool testSparseMatrix() {
    vector<vector<double>> A = {
        {4.0, 1.0, 0.0},
        {1.0, 3.0, 0.0},
        {0.0, 0.0, 2.0}
    };

    vector<vector<double>> L = incompleteCholesky(A);

    // Check if zeros are preserved
    if (L[0][2] != 0.0 || L[2][0] != 0.0) {
        cerr << "ICD did not preserve sparsity!" << endl;
        return false;
    }

    return true;
}

// Test ICD solve functionality
bool testICDSolve() {
    vector<vector<double>> A = {
        {4.0, 1.0},
        {1.0, 3.0}
    };
    vector<double> b = {1.0, 2.0};

    ICD icdSolver(A);
    vector<double> x = icdSolver.solve(b);

    // Check if A * x ≈ b
    vector<double> Ax = matVecMult(A, x);
    double error = sqrt(dotProduct(Ax, Ax)) - sqrt(dotProduct(b, b));  // Fixed line
    if (fabs(error) > 1e-6) {
        cerr << "ICD solve failed!" << endl;
        return false;
    }
    return true;
}

int main() {
    bool allTestsPassed = true;

    if (!testDenseMatrix()) {
        cerr << "Dense matrix test failed!" << endl;
        allTestsPassed = false;
    }

    if (!testSparseMatrix()) {
        cerr << "Sparse matrix test failed!" << endl;
        allTestsPassed = false;
    }

    if (!testICDSolve()) {
        cerr << "ICD solve test failed!" << endl;
        allTestsPassed = false;
    }

    if (allTestsPassed) {
        cout << "All ICD tests passed!" << endl;
        return 0;
    } else {
        return 1;
    }
}
