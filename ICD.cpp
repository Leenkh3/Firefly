#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>  // Added for exit() function

using namespace std;

// Dot product of two vectors
double dotProduct(const vector<double>& a, const vector<double>& b) {
    double result = 0.0;
    for (size_t i = 0; i < a.size(); i++) {
        result += a[i] * b[i];
    }
    return result;
}

// Vector addition: a = a + alpha * b
void vectorAdd(vector<double>& a, const vector<double>& b, double alpha) {
    for (size_t i = 0; i < a.size(); i++) {
        a[i] += alpha * b[i];
    }
}

// Vector subtraction: a = a - alpha * b
void vectorSub(vector<double>& a, const vector<double>& b, double alpha) {
    for (size_t i = 0; i < a.size(); i++) {
        a[i] -= alpha * b[i];
    }
}

// Function to compute matrix-vector product
vector<double> matVecMult(const vector<vector<double> > &A, const vector<double> &v) {
    int N = A.size();
    vector<double> result(N, 0.0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i] += A[i][j] * v[j];
        }
    }
    return result;
}

// Incomplete Cholesky decomposition function
vector<vector<double> > incompleteCholesky(const vector<vector<double> > &A) {
    int N = A.size();
    vector<vector<double> > L(N, vector<double>(N, 0.0));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
            if (i == j) {
                double sum = 0.0;
                for (int k = 0; k < j; k++) {
                    sum += L[j][k] * L[j][k];
                }
                L[j][j] = sqrt(A[j][j] - sum);
            } else {
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

// Forward substitution for solving L * y = b
vector<double> forwardSolve(const vector<vector<double> > &L, const vector<double> &b) {
    int N = L.size();
    vector<double> y(N, 0.0);
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
vector<double> backwardSolve(const vector<vector<double> > &L, const vector<double> &y) {
    int N = L.size();
    vector<double> x(N, 0.0);
    for (int i = N - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < N; j++) {
            sum += L[j][i] * x[j];
        }
        x[i] = (y[i] - sum) / L[i][i];
    }
    return x;
}

// Conjugate Gradient Solver
void conjugateGradient(const vector<vector<double> > &A, const vector<double> &b, vector<double> &x) {
    int N = A.size();
    vector<double> r = b; // Initial residual r0 = b - A*x0 (assuming x0 = 0)
    vector<double> p = r;
    vector<double> Ap(N);
    double alpha, beta, rsold, rsnew;

    rsold = dotProduct(r, r);
    for (int j = 0; j < N; j++) {
        Ap = matVecMult(A, p);
        alpha = rsold / dotProduct(p, Ap);

        vectorAdd(x, p, alpha);
        vectorSub(r, Ap, alpha);

        rsnew = dotProduct(r, r);
        if (sqrt(rsnew) < 1e-6) // Convergence check
            break;

        beta = rsnew / rsold;
        for (int i = 0; i < N; i++) {
            p[i] = r[i] + beta * p[i];
        }

        rsold = rsnew;
    }
}

int main() {
    int N;
    cout << "Enter the matrix size N: ";
    cin >> N;

    vector<vector<double> > A(N, vector<double>(N));
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

    vector<vector<double> > L = incompleteCholesky(A);
    conjugateGradient(A, b, x);

    cout << "Solution x: ";
    for (size_t i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }
    cout << endl;

    return 0;
}

