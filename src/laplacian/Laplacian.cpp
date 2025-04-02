// *****************************************************************************
/*!
  \file      src/Laplacian.hpp
  \brief     Functionality to compute the Laplacian operator
*/
// *****************************************************************************

#include <tuple>
#include <cassert>
#include <algorithm>
#include "Laplacian.hpp"


std::pair< std::vector< std::size_t >, std::vector< std::size_t > >
genEsup( const std::vector< std::size_t >& inpoel, std::size_t nnpe )
// *****************************************************************************
//  Generate derived data structure, elements surrounding points
//! \param[in] inpoel Inteconnectivity of points and elements. These are the
//!   node ids of each element of an unstructured mesh. Example:
//!   \code{.cpp}
//!     std::vector< std::size_t > inpoel { 12, 14,  9, 11,
//!                                         10, 14, 13, 12 };
//!   \endcode
//!   specifies two tetrahedra whose vertices (node ids) are { 12, 14, 9, 11 },
//!   and { 10, 14, 13, 12 }.
//! \param[in] nnpe Number of nodes per element
//! \return Linked lists storing elements surrounding points
//! \warning It is not okay to call this function with an empty container or a
//!   non-positive number of nodes per element; it will throw an exception.
//! \details The data generated here is stored in a linked list, more precisely,
//!   two linked arrays (vectors), _esup1_ and _esup2_, where _esup2_ holds the
//!   indices at which _esup1_ holds the element ids surrounding points. Looping
//!   over all elements surrounding all points can then be accomplished by the
//!   following loop:
//!   \code{.cpp}
//!     for (std::size_t p=0; p<npoin; ++p)
//!       for (auto i=esup.second[p]+1; i<=esup.second[p+1]; ++i)
//!          use element id esup.first[i]
//!   \endcode
//!     To find out the number of points, _npoin_, the mesh connectivity,
//!     _inpoel_, can be queried:
//!   \code{.cpp}
//!     auto minmax = std::minmax_element( begin(inpoel), end(inpoel) );
//!     assert( *minmax.first == 0); // node ids should start from zero
//!     auto npoin = *minmax.second + 1;
//!   \endcode
//! \note In principle, this function *should* work for any positive nnpe,
//!   however, only nnpe = 4 (tetrahedra) and nnpe = 3 (triangles) are tested.
//! \see Lohner, An Introduction to Applied CFD Techniques, Wiley, 2008
// *****************************************************************************
{
  assert( !inpoel.empty() ); // Attempt to call genEsup() on empty container
  assert( nnpe > 0 ); // Attempt to call genEsup() with zero nodes per element
  assert( inpoel.size()%nnpe == 0 ); // Size of inpoel must be divisible by nnpe

  // find out number of points in mesh connectivity
  auto minmax = std::minmax_element( begin(inpoel), end(inpoel) );
  assert( *minmax.first == 0 ); // node ids should start from zero
  auto npoin = *minmax.second + 1;

  // allocate one of the linked lists storing elements surrounding points: esup2
  // fill with zeros
  std::vector< std::size_t > esup2( npoin+1, 0 );

  // element pass 1: count number of elements connected to each point
  for (auto n : inpoel) ++esup2[ n + 1 ];

  // storage/reshuffling pass 1: update storage counter and store
  // also find out the maximum size of esup1 (mesup)
  auto mesup = esup2[0]+1;
  for (std::size_t i=1; i<npoin+1; ++i) {
    esup2[i] += esup2[i-1];
    if (esup2[i]+1 > mesup) mesup = esup2[i]+1;
  }

  // now we know mesup, so allocate the other one of the linked lists storing
  // elements surrounding points: esup1
  std::vector< std::size_t > esup1( mesup );

  // store the elements in esup1
  std::size_t e = 0;
  for (auto n : inpoel) {
    auto j = esup2[n]+1;
    esup2[n] = j;
    esup1[j] = e/nnpe;
    ++e;
  }

  // storage/reshuffling pass 2
  for (auto i=npoin; i>0; --i) esup2[i] = esup2[i-1];
  esup2[0] = 0;

  // Return (move out) linked lists
  return std::make_pair( std::move(esup1), std::move(esup2) );
}

std::pair< std::vector< std::size_t >, std::vector< std::size_t > >
genPsup( const std::vector< std::size_t >& inpoel,
         std::size_t nnpe,
         const std::pair< std::vector< std::size_t >,
                          std::vector< std::size_t > >& esup )
// *****************************************************************************
//  Generate derived data structure, points surrounding points
//! \param[in] inpoel Inteconnectivity of points and elements. These are the
//!   node ids of each element of an unstructured mesh. Example:
//!   \code{.cpp}
//!     std::vector< std::size_t > inpoel { 12, 14,  9, 11,
//!                                         10, 14, 13, 12 };
//!   \endcode
//!   specifies two tetrahedra whose vertices (node ids) are { 12, 14, 9, 11 },
//!   and { 10, 14, 13, 12 }.
//! \param[in] nnpe Number of nodes per element
//! \param[in] esup Elements surrounding points as linked lists, see tk::genEsup
//! \return Linked lists storing points surrounding points
//! \warning It is not okay to call this function with an empty container for
//!   inpoel or esup.first or esup.second or a non-positive number of nodes per
//!   element; it will throw an exception.
//! \details The data generated here is stored in a linked list, more precisely,
//!   two linked arrays (vectors), _psup1_ and _psup2_, where _psup2_ holds the
//!   indices at which _psup1_ holds the point ids surrounding points. Looping
//!   over all points surrounding all points can then be accomplished by the
//!   following loop:
//!   \code{.cpp}
//!     for (std::size_t p=0; p<npoin; ++p)
//!       for (auto i=psup.second[p]+1; i<=psup.second[p+1]; ++i)
//!          use point id psup.first[i]
//!   \endcode
//!    To find out the number of points, _npoin_, the mesh connectivity,
//!    _inpoel_, can be queried:
//!   \code{.cpp}
//!     auto minmax = std::minmax_element( begin(inpoel), end(inpoel) );
//!     assert( *minmax.first == 0 ); // node ids should start from zero
//!     auto npoin = *minmax.second + 1;
//!   \endcode
//!   or the length-1 of the generated index list:
//!   \code{.cpp}
//!     auto npoin = psup.second.size()-1;
//!   \endcode
//! \note In principle, this function *should* work for any positive nnpe,
//!   however, only nnpe = 4 (tetrahedra) and nnpe = 3 (triangles) are tested.
//! \see Lohner, An Introduction to Applied CFD Techniques, Wiley, 2008
// *****************************************************************************
{
  assert( !inpoel.empty() ); // Attempt to call genPsup() on empty container
  assert( nnpe > 0 ); // Attempt to call genPsup() with zero nodes per element
  assert( inpoel.size()%nnpe == 0 ); // Size of inpoel must be divisible by nnpe
  assert( !esup.first.empty() ); // Attempt to call genPsup() with empty esup1
  assert( !esup.second.empty() ); // Attempt to call genPsup() with empty esup2

  // find out number of points in mesh connectivity
  auto minmax = std::minmax_element( begin(inpoel), end(inpoel) );
  assert( *minmax.first == 0 ); // node ids should start from zero
  auto npoin = *minmax.second + 1;

  auto& esup1 = esup.first;
  auto& esup2 = esup.second;

  // allocate both of the linked lists storing points surrounding points, we
  // only know the size of psup2, put in a single zero in psup1
  std::vector< std::size_t > psup2( npoin+1 ), psup1( 1, 0 );

  // allocate and fill with zeros a temporary array, only used locally
  std::vector< std::size_t > lpoin( npoin, 0 );

  // fill both psup1 and psup2
  psup2[0] = 0;
  std::size_t j = 0;
  for (std::size_t p=0; p<npoin; ++p) {
    for (std::size_t i=esup2[p]+1; i<=esup2[p+1]; ++i ) {
      for (std::size_t n=0; n<nnpe; ++n) {
        auto q = inpoel[ esup1[i] * nnpe + n ];
        if (q != p && lpoin[q] != p+1) {
          ++j;
          psup1.push_back( q );
          lpoin[q] = p+1;
        }
      }
    }
    psup2[p+1] = j;
  }

  // sort point ids for each point in psup1
  for (std::size_t p=0; p<npoin; ++p)
    std::sort(
      std::next( begin(psup1), static_cast<std::ptrdiff_t>(psup2[p]+1) ),
      std::next( begin(psup1), static_cast<std::ptrdiff_t>(psup2[p+1]+1) ) );

  // Return (move out) linked lists
  return std::make_pair( std::move(psup1), std::move(psup2) );
}

//! Compute the cross-product of two vectors divided by a scalar
//! \param[in] v1 1st vector
//! \param[in] v2 2nd vector
//! \param[in] j Scalar to divide each component by
//! \return Cross-product divided by scalar
inline std::array< double, 3 >
crossdiv( const std::array< double, 3 >& v1,
          const std::array< double, 3 >& v2,
          double j )
{
  return {{ (v1[1]*v2[2] - v2[1]*v1[2]) / j,
            (v1[2]*v2[0] - v2[2]*v1[0]) / j,
            (v1[0]*v2[1] - v2[0]*v1[1]) / j }};
}

//! Compute the dot-product of two vectors
//! \param[in] v1 1st vector
//! \param[in] v2 2nd vector
//! \return Dot-product
inline double
dot( const std::array< double, 3 >& v1, const std::array< double, 3 >& v2 ) {
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

//! Compute the cross-product of two vectors
//! \param[in] v1x x coordinate of the 1st vector
//! \param[in] v1y y coordinate of the 1st vector
//! \param[in] v1z z coordinate of the 1st vector
//! \param[in] v2x x coordinate of the 2nd vector
//! \param[in] v2y y coordinate of the 2nd vector
//! \param[in] v2z z coordinate of the 2nd vector
//! \param[out] rx x coordinate of the product vector
//! \param[out] ry y coordinate of the product vector
//! \param[out] rz z coordinate of the product vector
inline void
cross( double v1x, double v1y, double v1z,
       double v2x, double v2y, double v2z,
       double& rx, double& ry, double& rz )
{
  rx = v1y*v2z - v2y*v1z;
  ry = v1z*v2x - v2z*v1x;
  rz = v1x*v2y - v2x*v1y;
}

//! Compute the cross-product of two vectors
//! \param[in] v1 1st vector
//! \param[in] v2 2nd vector
//! \return Cross-product
inline std::array< double, 3 >
cross( const std::array< double, 3 >& v1, const std::array< double, 3 >& v2 ) {
  double rx, ry, rz;
  cross( v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], rx, ry, rz );
  return { std::move(rx), std::move(ry), std::move(rz) };
}

//! Compute the triple-product of three vectors
//! \param[in] v1 1st vector
//! \param[in] v2 2nd vector
//! \param[in] v3 3rd vector
//! \return Triple-product
inline double
triple( const std::array< double, 3 >& v1,
        const std::array< double, 3 >& v2,
        const std::array< double, 3 >& v3 )
{
  return dot( v1, cross(v2,v3) );
}

std::tuple< SparseCSR, std::vector< double >, std::vector< double > >
laplacian( const std::vector< std::size_t >& inpoel,
           const std::array< std::vector< double >, 3 >& coord )
// *****************************************************************************
//  Setup matrix with Laplacian
//! \param[in] inpoel Mesh node connectivity
//! \param[in] coord Mesh node coordinates
//! \param[in] psup Points surrounding points
//! \return { A, x, b } in linear system A * x = b to solve
// *****************************************************************************
{
  const auto& X = coord[0];
  const auto& Y = coord[1];
  const auto& Z = coord[2];

  // compute points surrounding points
  auto psup = genPsup( inpoel, 4, genEsup(inpoel,4) );

  // Matrix with compressed sparse row storage
  SparseCSR A( inpoel, 4);



  // fill matrix with Laplacian
  for (std::size_t e=0; e<inpoel.size()/4; ++e) {
    const auto N = inpoel.data() + e*4;
    const std::array< double, 3 >
      ba{{ X[N[1]]-X[N[0]], Y[N[1]]-Y[N[0]], Z[N[1]]-Z[N[0]] }},
      ca{{ X[N[2]]-X[N[0]], Y[N[2]]-Y[N[0]], Z[N[2]]-Z[N[0]] }},
      da{{ X[N[3]]-X[N[0]], Y[N[3]]-Y[N[0]], Z[N[3]]-Z[N[0]] }};
    const auto J = triple( ba, ca, da );        // J = 6V
    assert( J > 0 ); // Element Jacobian non-positive
    std::array< std::array< double, 3 >, 4 > grad;
    grad[1] = crossdiv( ca, da, J );
    grad[2] = crossdiv( da, ba, J );
    grad[3] = crossdiv( ba, ca, J );
    for (std::size_t i=0; i<3; ++i)
      grad[0][i] = -grad[1][i]-grad[2][i]-grad[3][i];
    for (std::size_t a=0; a<4; ++a)
      for (std::size_t b=0; b<4; ++b)
         for (std::size_t k=0; k<3; ++k)
           A.at(N[a],N[b]) -= J/6.0 * grad[a][k] * grad[b][k];
  }

  auto nunk = X.size();
  std::vector< double > x( nunk, 0.0 ), b( nunk, 0.0 );

  return { std::move(A), std::move(x), std::move(b) };
}
