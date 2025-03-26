// *****************************************************************************
/*!
  \file      src/laplacian/Laplacian.hpp
  \brief     Functionality to compute the Laplacian operator
*/
// *****************************************************************************
#pragma once

#include <tuple>
#include <array>
#include <vector>

#include "CSR.hpp"

//! Generate derived data structure, elements surrounding points
std::pair< std::vector< std::size_t >, std::vector< std::size_t > >
genEsup( const std::vector< std::size_t >& inpoel, std::size_t nnpe );

//! Generate derived data structure, points surrounding points
std::pair< std::vector< std::size_t >, std::vector< std::size_t > >
genPsup( const std::vector< std::size_t >& inpoel,
         std::size_t nnpe,
         const std::pair< std::vector< std::size_t >,
                          std::vector< std::size_t > >& esup );

//  Setup matrix with Laplacian
std::tuple< CSR, std::vector< double >, std::vector< double > >
laplacian( const std::vector< std::size_t >& inpoel,
           const std::array< std::vector< double >, 3 >& coord );
