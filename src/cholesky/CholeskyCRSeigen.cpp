// CholeskyCRSeigen.cpp
#include "CholeskyCRSeigen.hpp"
#include <cassert>

// Compute the cross product divided by scalar (used in gradient calculation)
std::array<double, 3> crossdiv(const std::array<double, 3>& a, const std::array<double, 3>& b, double j) {
  return {
    (a[1]*b[2] - a[2]*b[1]) / j,
    (a[2]*b[0] - a[0]*b[2]) / j,
    (a[0]*b[1] - a[1]*b[0]) / j
  };
}

// Generate matrix A using Eigen::SparseMatrix and triplets
Eigen::SparseMatrix<double>
CholeskyCRSeigen(const std::vector<std::size_t>& inpoel,
                 const std::array<std::vector<double>, 3>& coord) {
  const auto& X = coord[0];
  const auto& Y = coord[1];
  const auto& Z = coord[2];

  std::size_t nnpe = 4; // nodes per element
  auto psup = genPsup(inpoel, nnpe, genEsup(inpoel, nnpe));
  std::size_t nunk = X.size();

  std::vector<Eigen::Triplet<double>> triplets;

  for (std::size_t e = 0; e < inpoel.size() / nnpe; ++e) {
    const auto N = inpoel.data() + e * nnpe;
 // Compute edge vectors for volume and gradient calculations
    std::array<double, 3> ba{{ X[N[1]] - X[N[0]], Y[N[1]] - Y[N[0]], Z[N[1]] - Z[N[0]] }};
    std::array<double, 3> ca{{ X[N[2]] - X[N[0]], Y[N[2]] - Y[N[0]], Z[N[2]] - Z[N[0]] }};
    std::array<double, 3> da{{ X[N[3]] - X[N[0]], Y[N[3]] - Y[N[0]], Z[N[3]] - Z[N[0]] }};
// Compute the volume of the tetrahedron using scalar triple product
    const double J = triple(ba, ca, da);
    assert(J > 0);

    std::array<std::array<double, 3>, 4> grad;
    grad[1] = crossdiv(ca, da, J);
    grad[2] = crossdiv(da, ba, J);
    grad[3] = crossdiv(ba, ca, J);

    for (std::size_t i = 0; i < 3; ++i)
      grad[0][i] = -grad[1][i] - grad[2][i] - grad[3][i];

    for (std::size_t a = 0; a < 4; ++a) {
      for (std::size_t b = 0; b < 4; ++b) {
        double value = 0.0;
        for (std::size_t k = 0; k < 3; ++k)
          value -= J / 6.0 * grad[a][k] * grad[b][k];
        triplets.emplace_back(N[a], N[b], value);
      }
    }
  }
// Assemble the global sparse matrix from triplets
  Eigen::SparseMatrix<double> A(nunk, nunk);
  A.setFromTriplets(triplets.begin(), triplets.end());
  A.makeCompressed();
  return A;
}
