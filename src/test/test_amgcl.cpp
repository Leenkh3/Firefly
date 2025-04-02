#include <iostream>
#include <tuple>
#include "matrix_generator.hpp"
#include <amgcl_solver.hpp>

int helper_poissonTest(
    int iters_exp,
    double error_exp,
    double error_delta,
    AmgclPrecondType precondType,
    std::string function_name = "")
{
    // Arrange
    const int ITERS_EXPECTED = iters_exp;
    const double ERROR_EXPECTED = error_exp;
    const double ERROR_DELTA = error_delta;

    const std::size_t size_orig = 100;
    std::vector<int> row_endpoints, col_indices;
    std::vector<double> values, rhs;

    int n = poisson(size_orig, row_endpoints, col_indices, values, rhs);

    int iters; double error;
    std::vector<double> x;

    // Act
    std::tie(iters, error) = solveAMGCL(
        precondType, row_endpoints, col_indices, values, rhs, x
    );

    int result = 0;
    // Assert
    if (iters != ITERS_EXPECTED)
    {
        result = 1;
    }
    if (error < ERROR_EXPECTED - ERROR_DELTA || error > ERROR_EXPECTED + ERROR_DELTA)
    {
        result = 2;
    }
    
    if (result > 0)
        std::cerr << function_name << std::endl;

    switch (result)
    {
        case 1:
            std::cerr << "Wrong number of iterations. Expected " << ITERS_EXPECTED <<
                " but got " << iters << std::endl;
        case 2:
            std::cerr << "Incorrect error. Expected " << ERROR_EXPECTED <<
                " but got " << error << std::endl;
        default:
            break;
    }

    if (result > 0)
        std::cerr << std::endl;

    return result;
}

int GivenPoissonMatrix_WithGaussSeidelPrecond_ItersAndErrorsMatchExpected()
{
    const int iters_exp = 6;
    const double error_exp = 1.35179e-09;
    const double error_delta = 1e-10;
    return helper_poissonTest(
        iters_exp, error_exp, error_delta, AmgclPrecond_GaussSeidel,
        "*** Poisson with Gauss-Seidel preconditioner ***");
}

int GivenPoissonMatrix_WithILU0Precond_ItersAndErrorsMatchExpected()
{
    const int iters_exp = 4;
    const double error_exp = 2.93818e-09;
    const double error_delta = 1e-10;
    return helper_poissonTest(
        iters_exp, error_exp, error_delta, AmgclPrecond_ILU0,
        "*** Poisson with ILU0 preconditioner ***");
}

int GivenPoissonMatrix_WithSPAI0Precond_ItersAndErrorsMatchExpected()
{
    const int iters_exp = 6;
    const double error_exp = 6.87251e-09;
    const double error_delta = 1e-10;
    return helper_poissonTest(
        iters_exp, error_exp, error_delta, AmgclPrecond_SPAI0,
        "*** Poisson with SPAI0 preconditioner ***");
}

int main()
{
    int result = 0;
    result += GivenPoissonMatrix_WithGaussSeidelPrecond_ItersAndErrorsMatchExpected();
    result += GivenPoissonMatrix_WithILU0Precond_ItersAndErrorsMatchExpected();
    result += GivenPoissonMatrix_WithSPAI0Precond_ItersAndErrorsMatchExpected();

    return result;
}