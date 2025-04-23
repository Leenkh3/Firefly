#include "ICD.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

// ===== Helper Functions in Anonymous Namespace =====
namespace {
    double dotProduct(const std::vector<double>& a, const std::vector<double>& b) {
        if (a.size() != b.size()) {
            throw std::runtime_error("dotProduct: Vector dimensions do not match.");
        }
        double result = 0.0;
        for (size_t i = 0; i < a.size(); i++) {
            result += a[i] * b[i];
        }
        return result;
    }

    std::vector<double> matVecMult(const std::vector<std::vector<double>>& A, const std::vector<double>& v) {
        if (A.empty() || A[0].size() != v.size()) {
            throw std::runtime_error("matVecMult: Matrix/vector dimensions incompatible.");
        }
        std::vector<double> result(A.size(), 0.0);
        for (size_t i = 0; i < A.size(); i++) {
            for (size_t j = 0; j < A[i].size(); j++) {
                result[i] += A[i][j] * v[j];
            }
        }
        return result;
    }

    // Forward substitution for solving L * y = b
    std::vector<double> forwardSolve(const std::vector<std::vector<double>>& L, const std::vector<double>& b) {
        int N = L.size();
        std::vector<double> y(N, 0.0);
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < i; j++) {
                sum += L[i][j] * y[j];
            }
            y[i] = (b[i] - sum) / L[i][i];
        }
        return y;
    }

    // Backward substitution for solving L^T * x = y
    std::vector<double> backwardSolve(const std::vector<std::vector<double>>& L, const std::vector<double>& y) {
        int N = L.size();
        std::vector<double> x(N, 0.0);
        for (int i = N - 1; i >= 0; i--) {
            double sum = 0.0;
            for (int j = i + 1; j < N; j++) {
                sum += L[j][i] * x[j];
            }
            x[i] = (y[i] - sum) / L[i][i];
        }
        return x;
    }
}

// ===== ICD Implementation =====
std::vector<std::vector<double>> incompleteCholesky(const std::vector<std::vector<double>>& A) {
    if (A.empty() || A.size() != A[0].size()) {
        throw std::runtime_error("incompleteCholesky: Matrix must be square.");
    }

    const int N = A.size();
    std::vector<std::vector<double>> L(N, std::vector<double>(N, 0.0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            // Skip zeros to preserve sparsity
            if (A[i][j] == 0.0) {
                L[i][j] = 0.0;
                continue;
            }

            if (i == j) {  // Diagonal elements
                double sum = 0.0;
                for (int k = 0; k < j; k++) {
                    sum += L[j][k] * L[j][k];
                }
                if (A[j][j] - sum <= 0.0) {
                    throw std::runtime_error("incompleteCholesky: Matrix is not positive definite.");
                }
                L[j][j] = sqrt(A[j][j] - sum);
            } else {  // Off-diagonal elements
                double sum = 0.0;
                for (int k = 0; k < j; k++) {
                    sum += L[i][k] * L[j][k];
                }
                L[i][j] = (A[i][j] - sum) / L[j][j];
            }
        }
    }
    return L;
}

ICD::ICD(const std::vector<std::vector<double>>& A) {
    if (A.empty()) {
        throw std::runtime_error("ICD: Input matrix is empty.");
    }
    L = incompleteCholesky(A);
}

std::vector<double> ICD::solve(const std::vector<double>& b) const {
    if (b.size() != L.size()) {
        throw std::runtime_error("ICD::solve: Vector size does not match matrix dimensions.");
    }

    // Use the forward/backward substitution from anonymous namespace
    std::vector<double> y = forwardSolve(L, b);
    std::vector<double> x = backwardSolve(L, y);
    return x;
}
