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
  return testLaplacian();
}


