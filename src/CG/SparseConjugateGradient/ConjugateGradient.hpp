#pragma once

#include <vector>
#include "SparseCSR.h"

// Conjugate Gradient solver for sparse linear system
void conjugateGradient(const SparseCSR& A, const std::vector<double>& b, std::vector<double>& x, double tol = 1e-6, std::size_t max_iter = 1000);
