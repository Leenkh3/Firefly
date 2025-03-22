#include <iostream>
#include "matrix_generator.hpp"
#include <amgcl_solver.hpp>

int ITERS_EXPECTED = 100;
double ERROR_EXPECTED = 0.0113451;
double ERROR_DELTA = 0.0001;

int main() {
    // Arrange
    const std::size_t size_orig = 100;
    std::vector<int> row_endpoints, col_indices;
    std::vector<double> values, rhs;

    int n = poisson(size_orig, row_endpoints, col_indices, values, rhs);

    int iters; double error;
    std::vector<double> x;

    // Act
    std::tie(iters, error) = solveAMGCL(
        row_endpoints, col_indices, values, rhs, x
    );

    // Assert
    if (iters != ITERS_EXPECTED)
    {
        std::cerr << "Wrong number of iterations. Expected " << ITERS_EXPECTED <<
            " but got " << iters << std::endl;
        return 1;
    }
    if (error < ERROR_EXPECTED - ERROR_DELTA || error > ERROR_EXPECTED + ERROR_DELTA)
    {
        std::cerr << "Incorrect error. Expected " << ERROR_EXPECTED <<
            " but got " << error << std::endl;
        return 1;
    }
    
    return 0;
}
