// *****************************************************************************
/*!
  \file      src/cholesky/testCholeskyCRSeigen.cpp
  \brief     Test Cholesky decomposition on mesh-based matrix using Eigen
*/
// *****************************************************************************

#include <iostream>
#include <vector>
#include <array>
#include <Eigen/Sparse>
#include "CholeskyCRSeigen.hpp"           // Generates the matrix A
#include "cholesky_CRS.hpp"               // CholeskySparseSolver

// Shift node IDs to start from zero
std::size_t shiftToZero(std::vector<std::size_t>& inpoel) {
  if (inpoel.empty()) return 0;
  std::size_t minId = *std::min_element(begin(inpoel), end(inpoel));
  for (auto& n : inpoel) n -= minId;
  return minId;
}

int testCholeskyCRSeigen() {
  // Mesh connectivity for tetrahedral mesh
  std::vector<std::size_t> inpoel {
    3,13,8,14, 12,3,13,8, 8,3,14,11, 12,3,8,11,
    1,2,3,13, 6,13,7,8, 5,9,14,11, 5,1,3,14,
    10,4,12,11, 2,6,12,13, 8,7,9,14, 13,1,7,14,
    5,3,4,11, 6,10,12,8, 3,2,4,12, 10,8,9,11,
    3,1,13,14, 13,7,8,14, 6,12,13,8, 9,8,14,11,
    3,5,14,11, 4,3,12,11, 3,2,12,13, 10,12,8,11
  };

  // Mesh node coordinates
  std::array<std::vector<double>, 3> coord {{
    {{-0.5, -0.5, -0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0, 0, 0, 0}},
    {{ 0.5,  0.5,  0,  -0.5, -0.5, 0.5, 0.5, 0, -0.5, -0.5, -0.5, 0, 0.5, 0}},
    {{-0.5,  0.5,  0,   0.5, -0.5, 0.5,-0.5, 0, -0.5,  0.5, 0, 0.5, 0, -0.5}}
  }};

  // Shift mesh node indices to start from 0
  shiftToZero(inpoel);

  // Generate the matrix A using the mesh
  Eigen::SparseMatrix<double> A = CholeskyCRSeigen(inpoel, coord);

  // Improve diagonal to enforce strict positive definiteness
for (int i = 0; i < A.outerSize(); ++i) {
  double rowSum = 0.0;
  for (Eigen::SparseMatrix<double>::InnerIterator it(A, i); it; ++it) {
    if (it.row() != it.col()) rowSum += std::abs(it.value());
  }
  A.coeffRef(i, i) = rowSum + 1.0; // strictly greater than sum of off-diagonals
}

  // Define the right-hand side vector b (ones)
  Eigen::VectorXd b(A.rows());
  b.setOnes();

  // Perform Cholesky decomposition and solve
  CholeskySparseSolver solver;
  if (!solver.compute(A)) {
    std::cerr << "Cholesky decomposition failed!\n";
    return -1;
  }


  Eigen::VectorXd x = solver.solve(b);
  Eigen::VectorXd Ax = A * x;

  // Check if Ax ≈ b
  if ((Ax - b).norm() > 1e-6) {
    std::cerr << "Cholesky solution incorrect. ||Ax - b|| = " << (Ax - b).norm() << "\n";
    return -1;
  }

  std::cout << " Cholesky test with mesh-based matrix PASSED!\n";
std::cout << "Solution x =\n" << x.transpose() << "\n";

  return 0;
}

int main() {
  return testCholeskyCRSeigen();
}

