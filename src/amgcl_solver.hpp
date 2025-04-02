#ifndef AMGCL_FIREFLY
#define AMGCL_FIREFLY

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
);

#endif