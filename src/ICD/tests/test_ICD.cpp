#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>  // for std::exit()

using namespace std;

// === ICD helper functions (assume these are declared in ICD.h) ===
vector<vector<double>> incompleteCholesky(const vector<vector<double>>& A);

// Test if L * Lᵗ ≈ A
bool testIncompleteCholesky(const vector<vector<double>>& A, double tolerance = 1e-6) {
    int N = A.size();
    vector<vector<double>> L = incompleteCholesky(A);

    // Reconstruct A from L * Lᵗ
    vector<vector<double>> A_reconstructed(N, vector<double>(N, 0.0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k <= min(i, j); ++k) {
                A_reconstructed[i][j] += L[i][k] * L[j][k];
            }
        }
    }

    // Compute error between original A and reconstructed A
    double error = 0.0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            error += fabs(A[i][j] - A_reconstructed[i][j]);
        }
    }

    cout << "ICD reconstruction error: " << error << endl;

    return error <= tolerance;
}

int main() {
    // Sample symmetric positive-definite matrix
    vector<vector<double>> A = {
        {4.0, 1.0, 1.0},
        {1.0, 3.0, 0.5},
        {1.0, 0.5, 2.0}
    };

    if (!testIncompleteCholesky(A)) {
        cerr << " ICD test failed!" << endl;
        exit(1); // Fail
    }

    cout << " ICD test passed!" << endl;
    // No return statement — so nothing is returned if success
}
