#define AMGCL_NO_BOOST

#include <vector>
#include <amgcl/amg.hpp>
#include <amgcl/backend/builtin.hpp>
#include <amgcl/make_solver.hpp>
#include <amgcl/solver/cg.hpp>
#include <amgcl/solver/bicgstab.hpp>
#include <amgcl/adapter/crs_tuple.hpp>
#include <amgcl/coarsening/smoothed_aggregation.hpp>
#include <amgcl/relaxation/as_preconditioner.hpp>
#include <amgcl/relaxation/damped_jacobi.hpp>
#include <amgcl/relaxation/spai0.hpp>

/*
Input:
- row_endpoints: vector of row endpoints of matrix in CRS format
- col_indices: vector of column indices of matrix in CRS format
- values: vector of values of matrix in CRS format
- rhs: vector representing the right-hand-side
- x: used as output, vector representing solution after algorithm finished
Returns: tuple of number of iterations used and the error.
*/
std::tuple<int, double> solveAMGCL(
    const std::vector<int> &row_endpoints,
    const std::vector<int> &col_indices,
    const std::vector<double> &values,
    const std::vector<double> &rhs,
    std::vector<double> &x
)
{
    int n = row_endpoints.size() - 1;

    typedef amgcl::backend::builtin<double> Backend;

    // Define the solver type
    typedef amgcl::make_solver<
        amgcl::relaxation::as_preconditioner<
            Backend,
            amgcl::relaxation::damped_jacobi
        >,
        amgcl::solver::cg<Backend>
    > Solver;

    // Kept original solver here - it has much lower error.
    // TODO: introduce a factory pattern to create the solver based on user input (as in issue).
    /*
    typedef amgcl::make_solver<
        amgcl::amg<
            Backend,
            amgcl::coarsening::smoothed_aggregation,
            amgcl::relaxation::spai0
        >,
        amgcl::solver::bicgstab<Backend>
    > Solver_orig;
    */

    // Create the solver
    Solver solve(std::tie(n, row_endpoints, col_indices, values));

    // Solve the system
    if (x.size() != n)
        x.assign(n, 0);

    return solve(rhs, x);
}