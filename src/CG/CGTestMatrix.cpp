#include "CGMatrix.h"
#include <cassert>
#include <iostream>

void testConjugateGradient() {
    std::vector<std::vector<double>> A = {{4, 1}, {1, 3}};
    std::vector<double> b = {1, 2};
    std::vector<double> x = {0, 0};

    conjugateGradient(A, b, x);

    double tol = 1e-4;
    assert(std::abs(x[0] - 0.0909) < tol && "x[0] incorrect");
    assert(std::abs(x[1] - 0.6364) < tol && "x[1] incorrect");
    std::cout << "Conjugate Gradient test passed" << std::endl;
}

int main() {
    testConjugateGradient();
    return 0;
}
