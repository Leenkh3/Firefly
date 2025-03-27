// *****************************************************************************
/*!
  \file      src/laplacian/CSR.hpp
  \brief     Compressed sparse row (CSR) storage for a sparse matrix
*/
// *****************************************************************************
#pragma once

#include <iostream>
#include <vector>

//! Compressed sparse row (CSR) storage for a sparse matrix
class CSR {

  public:
    //! \brief Constructor: Create a CSR symmetric matrix with one scalar
    //!   component, storing only the upper triangular part
    explicit CSR( const std::pair< std::vector< std::size_t >,
                                   std::vector< std::size_t > >& psup );

    //! Return const reference to sparse matrix entry
    const double&
    operator()( std::size_t row, std::size_t col ) const;

    //! Return non-const reference to sparse matrix entry at a position
    //! \see "Avoid Duplication in const and Non-const Member Function," and
    //!   "Use const whenever possible," Scott Meyers, Effective C++, 3d ed.
    double&
    operator()( std::size_t row, std::size_t col ) {
      return const_cast< double& >(
               static_cast< const CSR& >( *this ).operator()( row, col ) );
    }

    //! Set Dirichlet boundary condition at a node
    void dirichlet( std::size_t i,
                    double val,
                    std::vector< double >& b );

    //! Multiply CSR matrix with vector from the right: r = A * x
    void mult( const std::vector< double >& x, std::vector< double >& r ) const;

    //! Zero matrix values
    void zero();

    //! Access real size of matrix
    std::size_t rsize() const { return rnz.size(); }

    //! Write out CSR as stored
    std::ostream& write_stored( std::ostream &os ) const;
    //! Write out CSR nonzero structure
    std::ostream& write_structure( std::ostream &os ) const;
    //! Write out CSR contribute structure
    std::ostream& write_contribute( std::ostream &os ) const;
    //! Write out CSR as a real matrix
    std::ostream& write_matrix( std::ostream &os ) const;
    //! Write out CSR in Matlab/Octave format
    std::ostream& write_matlab( std::ostream &os ) const;

  private:
    std::vector< std::size_t > rnz;     //!< Number of nonzeros in each row
    std::vector< std::size_t > ia;      //!< Row pointers
    std::vector< std::size_t > ja;      //!< Column indices
    std::vector< double > a;            //!< Nonzero matrix values
};
