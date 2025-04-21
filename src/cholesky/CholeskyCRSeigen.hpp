// *****************************************************************************
//  file      src/cholesky/CholeskyCRSeigen.hpp
//  brief     Header for generating sparse matrix from mesh and solving with Cholesky
// *****************************************************************************
#pragma once

#include <vector>
#include <array>
#include <Eigen/Sparse>

// Cross product of two 3D vectors
inline std::array<double, 3> cross(const std::array<double, 3>& a, const std::array<double, 3>& b) {
  return {
    a[1]*b[2] - a[2]*b[1],
    a[2]*b[0] - a[0]*b[2],
    a[0]*b[1] - a[1]*b[0]
  };
}

// Dot product of two 3D vectors
inline double dot(const std::array<double, 3>& a, const std::array<double, 3>& b) {
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

// Scalar triple product: a · (b × c)
inline double triple(const std::array<double, 3>& a,
                     const std::array<double, 3>& b,
                     const std::array<double, 3>& c) {
  return dot(a, cross(b, c));
}



// ------
std::pair<std::vector<std::size_t>, std::vector<std::size_t>>
genEsup(const std::vector<std::size_t>& inpoel, std::size_t nnpe);

std::pair<std::vector<std::size_t>, std::vector<std::size_t>>
genPsup(const std::vector<std::size_t>& inpoel,
        std::size_t nnpe,
        const std::pair<std::vector<std::size_t>, std::vector<std::size_t>>& esup);

std::array<double, 3> crossdiv(const std::array<double, 3>& v1,
                               const std::array<double, 3>& v2,
                               double j);

double dot(const std::array<double, 3>& v1, const std::array<double, 3>& v2);

std::array<double, 3> cross(const std::array<double, 3>& v1,
                            const std::array<double, 3>& v2);

double triple(const std::array<double, 3>& v1,
              const std::array<double, 3>& v2,
              const std::array<double, 3>& v3);

// ----------------------------------------------------------------------------
// Main function to generate a sparse matrix from mesh using Eigen triplets
Eigen::SparseMatrix<double> CholeskyCRSeigen(
  const std::vector<std::size_t>& inpoel,
  const std::array<std::vector<double>, 3>& coord);

