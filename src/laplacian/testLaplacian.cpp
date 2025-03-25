// *****************************************************************************
/*!
  \file      src/laplacian/testLaplacian.cpp
  \brief     Generate and test a Laplacian operator
*/
// *****************************************************************************

#include <cstddef>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "../laplacian/Laplacian.hpp"

std::size_t
shiftToZero( std::vector< std::size_t >& inpoel )
// *****************************************************************************
//  Shift node IDs to start with zero in element connectivity
//! \param[inout] inpoel Inteconnectivity of points and elements
//! \return Amount shifted
//! \details This function implements a simple reordering of the node ids of the
//!   element connectivity in inpoel by shifting the node ids so that the
//!   smallest is zero.
//! \note It is okay to call this function with an empty container; it will
//!    simply return without throwing an exception.
// *****************************************************************************
{
  if (inpoel.empty()) return 0;

  // find smallest node id
  auto minId = *std::min_element( begin(inpoel), end(inpoel) );

  // shift node ids to start from zero
  for (auto& n : inpoel) n -= minId;

  return minId;
}

int
testCSR()
// *****************************************************************************
// Test CSR matrix storage
// *****************************************************************************
{
  // Mesh connectivity for simple tetrahedron-only mesh
  std::vector< std::size_t > inpoel { 12, 14,  9, 11,
                                      10, 14, 13, 12,
                                      14, 13, 12,  9,
                                      10, 14, 12, 11,
                                      1,  14,  5, 11,
                                      7,   6, 10, 12,
                                      14,  8,  5, 10,
                                      8,   7, 10, 13,
                                      7,  13,  3, 12,
                                      1,   4, 14,  9,
                                      13,  4,  3,  9,
                                      3,   2, 12,  9,
                                      4,   8, 14, 13,
                                      6,   5, 10, 11,
                                      1,   2,  9, 11,
                                      2,   6, 12, 11,
                                      6,  10, 12, 11,
                                      2,  12,  9, 11,
                                      5,  14, 10, 11,
                                      14,  8, 10, 13,
                                      13,  3, 12,  9,
                                      7,  10, 13, 12,
                                      14,  4, 13,  9,
                                      14,  1,  9, 11 };

  // Mesh node coordinates for simple tet mesh above
  std::array< std::vector< double >, 3 > coord {{
    {{ 0, 1, 1, 0, 0, 1, 1, 0, 0.5, 0.5, 0.5, 1,   0.5, 0 }},
    {{ 0, 0, 1, 1, 0, 0, 1, 1, 0.5, 0.5, 0,   0.5, 1,   0.5 }},
    {{ 0, 0, 0, 0, 1, 1, 1, 1, 0,   1,   0.5, 0.5, 0.5, 0.5 }} }};

  // Shift node IDs to start from zero
  shiftToZero( inpoel );

  // Generate points surrounding points
  auto psup = genPsup( inpoel, 4, genEsup(inpoel,4) );

  // Create matrix
  CSR c( psup );

  // Output sparse matrix data to stringstream
  std::stringstream ss;
  c.write_stored( ss );

  auto correct = R"(size (npoin) = 14
ncomp = 1
rsize (size) = 14
nnz = 112
rnz[npoin=14] = { 7, 7, 7, 7, 7, 7, 7, 7, 9, 9, 9, 10, 9, 10 }
ia[rsize+1=15] = { 1, 8, 15, 22, 29, 36, 43, 50, 57, 66, 75, 84, 94, 103, 113 }
ja[nnz=112] = { 1, 2, 4, 5, 9, 11, 14, 1, 2, 3, 6, 9, 11, 12, 2, 3, 4, 7, 9, 12, 13, 1, 3, 4, 8, 9, 13, 14, 1, 5, 6, 8, 10, 11, 14, 2, 5, 6, 7, 10, 11, 12, 3, 6, 7, 8, 10, 12, 13, 4, 5, 7, 8, 10, 13, 14, 1, 2, 3, 4, 9, 11, 12, 13, 14, 5, 6, 7, 8, 10, 11, 12, 13, 14, 1, 2, 5, 6, 9, 10, 11, 12, 14, 2, 3, 6, 7, 9, 10, 11, 12, 13, 14, 3, 4, 7, 8, 9, 10, 12, 13, 14, 1, 4, 5, 8, 9, 10, 11, 12, 13, 14 }
a[nnz=112] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
)";

  // test CSR matrix data
  if (ss.str() != correct ) {
    std::cerr << "CSR write_stored incorrect";
    return -1;
  }

  return 0;
}

int
testLaplacian()
// *****************************************************************************
// Test Laplace operator
// *****************************************************************************
{
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
  shiftToZero( inpoel );

  // Fill matrix with Laplace operator values
  auto [A,x,b] = laplacian( inpoel, coord );

  // Output sparse matrix data to stringstream
  std::stringstream ss;
  A.write_matlab( ss );
  //A.write_matlab( std::cout );

  auto correct = R"(A = [ -0.5 -0.0416667 0.208333 0 -0.0416667 0 -0.0416667 0 0 0 0 0 0.208333 0.208333 ;
-0.0416667 -0.5 0.208333 -0.0416667 0 -0.0416667 0 0 0 0 0 0.208333 0.208333 0 ;
0.208333 0.208333 -1.66667 0.208333 0.208333 0 0 -0.166667 0 0 0.25 0.25 0.25 0.25 ;
0 -0.0416667 0.208333 -0.5 -0.0416667 0 0 0 0 -0.0416667 0.208333 0.208333 0 0 ;
-0.0416667 0 0.208333 -0.0416667 -0.5 0 0 0 -0.0416667 0 0.208333 0 0 0.208333 ;
0 -0.0416667 0 0 0 -0.5 -0.0416667 0.208333 0 -0.0416667 0 0.208333 0.208333 0 ;
-0.0416667 0 0 0 0 -0.0416667 -0.5 0.208333 -0.0416667 0 0 0 0.208333 0.208333 ;
0 0 -0.166667 0 0 0.208333 0.208333 -1.66667 0.208333 0.208333 0.25 0.25 0.25 0.25 ;
0 0 0 0 -0.0416667 0 -0.0416667 0.208333 -0.5 -0.0416667 0.208333 0 0 0.208333 ;
0 0 0 -0.0416667 0 -0.0416667 0 0.208333 -0.0416667 -0.5 0.208333 0.208333 0 0 ;
0 0 0.25 0.208333 0.208333 0 0 0.25 0.208333 0.208333 -1.5 0.0833333 0 0.0833333 ;
0 0.208333 0.25 0.208333 0 0.208333 0 0.25 0 0.208333 0.0833333 -1.5 0.0833333 0 ;
0.208333 0.208333 0.25 0 0 0.208333 0.208333 0.25 0 0 0 0.0833333 -1.5 0.0833333 ;
0.208333 0 0.25 0 0.208333 0 0.208333 0.25 0.208333 0 0.0833333 0 0.0833333 -1.5 ;
]
)";

  // test Laplace matrix data
  if (ss.str() != correct ) {
    std::cerr << "Laplace operator write_matlab incorrect";
    return -1;
  }

  return 0;
}

int
main(int argc, char * argv[])
// *****************************************************************************
// Test main
// *****************************************************************************
{
  return testCSR() | testLaplacian();
}
