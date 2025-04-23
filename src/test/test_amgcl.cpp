#include <iostream>
#include <algorithm>
#include <tuple>
#include <amgcl_solver.hpp>
#include "matrix_generator.hpp"
#include "Laplacian.hpp"

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

// Taken from testLaplacian, as the same setup is used for checking AMGCL solver integration
int helper_laplaceTest(
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

    // Mesh connectivity for simple tetrahedron-only mesh
    std::vector< std::size_t > inpoel {
        3, 13, 8, 14,
        12, 3, 13, 8,
        8, 3, 14, 11,
        12, 3, 8, 11,
        1, 2, 3, 13,
        6, 13, 7, 8,
        5, 9, 14, 11,
        5, 1, 3, 14,
        10, 4, 12, 11,
        2, 6, 12, 13,
        8, 7, 9, 14,
        13, 1, 7, 14,
        5, 3, 4, 11,
        6, 10, 12, 8,
        3, 2, 4, 12,
        10, 8, 9, 11,
        3, 1, 13, 14,
        13, 7, 8, 14,
        6, 12, 13, 8,
        9, 8, 14, 11,
        3, 5, 14, 11,
        4, 3, 12, 11,
        3, 2, 12, 13,
        10, 12, 8, 11 };

    // Mesh node coordinates for simple tet mesh above
    std::array< std::vector< double >, 3 > coord {{
        {{ -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0 }},
        {{ 0.5, 0.5, 0, -0.5, -0.5, 0.5, 0.5, 0, -0.5, -0.5, -0.5, 0, 0.5, 0 }},
        {{ -0.5, 0.5, 0, 0.5, -0.5, 0.5, -0.5, 0, -0.5, 0.5, 0, 0.5, 0, -0.5 }} }};

    // Shift node IDs to start from zero
    if (inpoel.empty()) return 0;
    // find smallest node id
    auto minId = *std::min_element( begin(inpoel), end(inpoel) );
    // shift node ids to start from zero
    for (auto& n : inpoel) n -= minId;
    
    // Fill matrix with Laplace operator values
    auto [A, x, b] = laplacian(inpoel, coord);

    // Set Dirichlet BC
    A.dirichlet(0, 1, b);


    // Act - check laplace with this input
    int iters; double error;
    std::tie(iters, error) = solveAMGCL(precondType, A, x, b);
    std::cerr << "LAPLACE iter: " << iters << " error: " << error << std::endl;

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
        std::cerr << function_name;

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

int GivenLaplaceInput_WithGaussSeidelPrecond_ItersAndErrorsMatchExpected()
{
    const int iters_exp = 1;
    const double error_exp = 9.33731e-16;
    const double error_delta = 3e-16;
    return helper_laplaceTest(
        iters_exp, error_exp, error_delta, AmgclPrecond_GaussSeidel,
        "*** Laplace with Gauss-Seidel preconditioner ***");
}

int GivenLaplaceInput_WithILU0Precond_ItersAndErrorsMatchExpected()
{
    const int iters_exp = 1;
    const double error_exp = 9.33731e-16;
    const double error_delta = 3e-16;
    return helper_laplaceTest(
        iters_exp, error_exp, error_delta, AmgclPrecond_ILU0,
        "*** Laplace with ILU0 preconditioner ***");
}

int GivenLaplaceInput_WithSPAI0Precond_ItersAndErrorsMatchExpected()
{
    const int iters_exp = 1;
    const double error_exp = 9.33731e-16;
    const double error_delta = 3e-16;
    return helper_laplaceTest(
        iters_exp, error_exp, error_delta, AmgclPrecond_SPAI0,
        "*** Laplace with SPAI0 preconditioner ***");
}

int main()
{
    int result = 0;
    result += GivenPoissonMatrix_WithGaussSeidelPrecond_ItersAndErrorsMatchExpected();
    result += GivenPoissonMatrix_WithILU0Precond_ItersAndErrorsMatchExpected();
    result += GivenPoissonMatrix_WithSPAI0Precond_ItersAndErrorsMatchExpected();
    result += GivenLaplaceInput_WithGaussSeidelPrecond_ItersAndErrorsMatchExpected();
    result += GivenLaplaceInput_WithILU0Precond_ItersAndErrorsMatchExpected();
    result += GivenLaplaceInput_WithSPAI0Precond_ItersAndErrorsMatchExpected();

    return result;
}