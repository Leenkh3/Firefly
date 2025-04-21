// *****************************************************************************
//  file      src/cholesky/CholeskyCRSeigen.hpp
//  brief     Header for generating sparse matrix from mesh and solving with Cholesky
// *****************************************************************************
#pragma once

#include <vector>
#include <array>
#include <Eigen/Sparse>

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

