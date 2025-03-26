#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Function to compute matrix-vector product
vector<double> matVecMult(const vector<vector<double>> &A, const vector<double> &v) {
    int N = A.size();
    vector<double> result(N, 0.0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i] += A[i][j] * v[j];
        }
    }
    return result;
}

// Function to compute dot product of two vectors
double dotProduct(const vector<double> &v1, const vector<double> &v2) {
    double sum = 0.0;
    for (size_t i = 0; i < v1.size(); i++) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

// Function to compute vector addition: v1 = v1 + alpha * v2
void vectorAdd(vector<double> &v1, const vector<double> &v2, double alpha) {
    for (size_t i = 0; i < v1.size(); i++) {
        v1[i] += alpha * v2[i];
    }
}

// Function to compute vector subtraction: v1 = v1 - alpha * v2
void vectorSub(vector<double> &v1, const vector<double> &v2, double alpha) {
    for (size_t i = 0; i < v1.size(); i++) {
        v1[i] -= alpha * v2[i];
    }
}

// Incomplete Cholesky Decomposition
vector<vector<double>> incompleteCholesky(const vector<vector<double>> &A) {
    int N = A.size();
    vector<vector<double>> L(N, vector<double>(N, 0.0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = A[i][j];
            for (int k = 0; k < j; k++) {
                sum -= L[i][k] * L[j][k];
            }
            if (i == j) {
                if (sum <= 0) {
                    cerr << "Matrix is not positive definite!" << endl;
                    exit(1);
                }
                L[i][j] = sqrt(sum);
            } else {
                L[i][j] = sum / L[j][j];
            }
        }
    }
    return L;
}

// Function to solve L*y = b
vector<double> forwardSolve(const vector<vector<double>> &L, const vector<double> &b) {
    int N = L.size();
    vector<double> y(N, 0.0);

    for (int i = 0; i < N; i++) {
        double sum = b[i];
        for (int j = 0; j < i; j++) {
            sum -= L[i][j] * y[j];
        }
        y[i] = sum / L[i][i];
    }
    return y;
}

// Function to solve L^T*x = y
vector<double> backwardSolve(const vector<vector<double>> &L, const vector<double> &y) {
    int N = L.size();
    vector<double> x(N, 0.0);

    for (int i = N - 1; i >= 0; i--) {
        double sum = y[i];
        for (int j = i + 1; j < N; j++) {
            sum -= L[j][i] * x[j];
        }
        x[i] = sum / L[i][i];
    }
    return x;
}

// Preconditioned Conjugate Gradient Solver
void conjugateGradient(const vector<vector<double>> &A, const vector<double> &b, vector<double> &x) {
    int N = A.size();

    // Compute Incomplete Cholesky Decomposition
    vector<vector<double>> L = incompleteCholesky(A);

    // Initial residual r0 = b - A*x0 (assuming x0 = 0)
    vector<double> r = b;
    vector<double> z = forwardSolve(L, r); // Preconditioned residual
    z = backwardSolve(L, z);
    vector<double> p = z;
    vector<double> Ap(N);
    double alpha, beta, rsold, rsnew;

    rsold = dotProduct(r, z);
    for (int j = 0; j < N; j++) {
        Ap = matVecMult(A, p);
        alpha = rsold / dotProduct(p, Ap);

        vectorAdd(x, p, alpha); // Update x
        vectorSub(r, Ap, alpha); // Update r

        z = forwardSolve(L, r); // Apply preconditioner
        z = backwardSolve(L, z);

        rsnew = dotProduct(r, z);
        if (sqrt(rsnew) < 1e-6) // Convergence check
            break;

        beta = rsnew / rsold;
        for (int i = 0; i < N; i++) {
            p[i] = z[i] + beta * p[i];
        }

        rsold = rsnew;
    }
}

int main() {
    int N;
    cout << "Enter the matrix size N: ";
    cin >> N;

    vector<vector<double>> A(N, vector<double>(N));
    vector<double> b(N);
    vector<double> x(N, 0.0);

    cout << "Enter the " << N << "x" << N << " matrix A (row by row):\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> A[i][j];
        }
    }

    cout << "Enter the right-hand side vector b:\n";
    for (int i = 0; i < N; i++) {
        cin >> b[i];
    }

    conjugateGradient(A, b, x);

    cout << "Solution x: ";
    for (size_t i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }
    cout << endl;

    return 0;
}

