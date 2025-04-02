#define AMGCL_NO_BOOST

#include <vector>
#include <amgcl/amg.hpp>
#include <amgcl/backend/builtin.hpp>
#include <amgcl/make_solver.hpp>
#include <amgcl/solver/cg.hpp>
#include <amgcl/solver/bicgstab.hpp>
#include <amgcl/adapter/crs_tuple.hpp>
#include <amgcl/coarsening/smoothed_aggregation.hpp>
#include <amgcl/relaxation/gauss_seidel.hpp>
#include <amgcl/relaxation/ilu0.hpp>
#include <amgcl/relaxation/spai0.hpp>

#include <amgcl_solver.hpp>

typedef amgcl::backend::builtin<double> Backend;

typedef amgcl::make_solver<
    amgcl::amg<
        Backend,
        amgcl::coarsening::smoothed_aggregation,
        amgcl::relaxation::gauss_seidel>,
    amgcl::solver::bicgstab<Backend>>
    Solver_G_S;

typedef amgcl::make_solver<
    amgcl::amg<
        Backend,
        amgcl::coarsening::smoothed_aggregation,
        amgcl::relaxation::ilu0>,
    amgcl::solver::bicgstab<Backend>>
    Solver_ILU0;

typedef amgcl::make_solver<
    amgcl::amg<
        Backend,
        amgcl::coarsening::smoothed_aggregation,
        amgcl::relaxation::spai0>,
    amgcl::solver::bicgstab<Backend>>
    Solver_SPAI0;

/*
Input:
- preconditioner: AmgclPrecondType enum value specifying preconditioner
- row_endpoints: vector of row endpoints of matrix in CRS format
- col_indices: vector of column indices of matrix in CRS format
- values: vector of values of matrix in CRS format
- rhs: vector representing the right-hand-side
- x: used as output, vector representing solution after algorithm finished
Returns: tuple of number of iterations used and the error.
*/
SolverResult solveAMGCL(
    const AmgclPrecondType &preconditioner,
    const std::vector<int> &row_endpoints,
    const std::vector<int> &col_indices,
    const std::vector<double> &values,
    const std::vector<double> &rhs,
    std::vector<double> &x
)
{
    int n = row_endpoints.size() - 1;


    if (x.size() != n)
        x.assign(n, 0);

    // Define the solver type
    if (preconditioner == AmgclPrecond_GaussSeidel)
    {
        Solver_G_S solve(std::tie(n, row_endpoints, col_indices, values));
        return solve(rhs, x);
    }
    else if (preconditioner == AmgclPrecond_ILU0)
    {
        Solver_ILU0 solve(std::tie(n, row_endpoints, col_indices, values));
        return solve(rhs, x);
    }
    else if (preconditioner == AmgclPrecond_SPAI0)
    {
        Solver_SPAI0 solve(std::tie(n, row_endpoints, col_indices, values));
        return solve(rhs, x);
    }
    else
    {
        throw -1;
    }
}