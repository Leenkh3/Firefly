#include <cmath>
#include "ConjugateGradient.hpp"

void conjugateGradient(const SparseCSR& A, const std::vector<double>& b, std::vector<double>& x, double tol, std::size_t max_iter) {
    std::size_t N = x.size();
    std::vector<double> r = b; // Initial residual r = b - A*x (x initially 0)
    std::vector<double> p = r; // Initial search direction
    std::vector<double> Ap(N);
    double rsold = 0.0, rsnew = 0.0, alpha = 0.0, beta = 0.0;

    // Compute initial residual if x is not zero: r = b - A*x
    Ap = A.mult(x);
    for (std::size_t i = 0; i < N; ++i) {
        r[i] -= Ap[i];
    }

    rsold = 0.0;
    for (std::size_t i = 0; i < N; ++i) {
        rsold += r[i] * r[i];
    }

    for (std::size_t iter = 0; iter < max_iter; ++iter) {
        Ap = A.mult(p); // Matrix-vector product
        double pAp = 0.0;
        for (std::size_t i = 0; i < N; ++i) {
            pAp += p[i] * Ap[i];
        }
        alpha = rsold / pAp;

        // Update solution: x = x + alpha * p
        for (std::size_t i = 0; i < N; ++i) {
            x[i] += alpha * p[i];
        }

        // Update residual: r = r - alpha * Ap
        for (std::size_t i = 0; i < N; ++i) {
            r[i] -= alpha * Ap[i];
        }

        rsnew = 0.0;
        for (std::size_t i = 0; i < N; ++i) {
            rsnew += r[i] * r[i];
        }

        if (std::sqrt(rsnew) < tol) {
            std::cout << "CG converged in " << iter + 1 << " iterations\n";
            break;
        }

        beta = rsnew / rsold;
        // Update search direction: p = r + beta * p
        for (std::size_t i = 0; i < N; ++i) {
            p[i] = r[i] + beta * p[i];
        }

        rsold = rsnew;
    }
}
