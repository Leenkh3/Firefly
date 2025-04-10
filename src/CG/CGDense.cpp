#include "CGMatrix.h"
#include <cmath>

using namespace std;

vector<double> matVecMult(const vector<vector<double>>& A, const vector<double>& v) {
    int N = A.size();
    vector<double> result(N, 0.0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            result[i] += A[i][j] * v[j];
        }
    }
    return result;
}

double dotProduct(const vector<double>& v1, const vector<double>& v2) {
    double sum = 0.0;
    for (size_t i = 0; i < v1.size(); i++) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

void vectorAdd(vector<double>& v1, const vector<double>& v2, double alpha) {
    for (size_t i = 0; i < v1.size(); i++) {
        v1[i] += alpha * v2[i];
    }
}

void vectorSub(vector<double>& v1, const vector<double>& v2, double alpha) {
    for (size_t i = 0; i < v1.size(); i++) {
        v1[i] -= alpha * v2[i];
    }
}

void conjugateGradient(const vector<vector<double>>& A, const vector<double>& b, vector<double>& x) {
    int N = A.size();
    vector<double> r = b;
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
        if (sqrt(rsnew) < 1e-6)
            break;

        beta = rsnew / rsold;
        for (int i = 0; i < N; i++) {
            p[i] = r[i] + beta * p[i];
        }
        rsold = rsnew;
    }
}
