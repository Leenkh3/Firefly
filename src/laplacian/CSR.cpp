// *****************************************************************************
/*!
  \file      src/laplacian/CSR.cpp
  \brief     Compressed sparse row (CSR) storage for a sparse matrix
*/
// *****************************************************************************

#include <cassert>

#include "CSR.hpp"

CSR::CSR( const std::pair< std::vector< std::size_t >,
                           std::vector< std::size_t > >& psup )
try :
  rnz( psup.second.size()-1 ),
  ia( rnz.size()+1 )
// *****************************************************************************
//  Constructor: Create a CSR symmetric matrix with 1 scalar component per
//  non-zero matrix entry, storing only the upper triangular part
//! \param[in] nc Number of scalar components (degrees of freedom)
//! \param[in] psup Points surrounding points of mesh graph, see tk::genPsup
// *****************************************************************************
{
  assert( rnz.size() > 0 ); // Sparse matrix size must be positive

  const auto& psup1 = psup.first;
  const auto& psup2 = psup.second;

  // Calculate number of nonzeros in each block row (rnz), total number of
  // nonzeros (nnz), and fill in row indices (ia)
  std::size_t nnz, i;
  for (ia[0]=1, nnz=i=0; i<psup2.size()-1; ++i) {
    // add up and store nonzeros of row i (only upper triangular part)
    std::size_t j;
    for (rnz[i]=1, j=psup2[i]+1; j<=psup2[i+1]; ++j)
      ++rnz[i];

    // add up total number of nonzeros
    nnz += rnz[i];

    // fill up row index
    ia[i+1] = ia[i] + rnz[i];
  }

  // Allocate storage for matrix values and column indices
  a.resize( nnz, 0.0 );
  ja.resize( nnz );

  // fill column indices
  for (i=0; i<rnz.size(); ++i) {
    auto itmp = i;
    ja[ia[itmp]-1] = itmp+1;  // put in column index of diagonal
    for (std::size_t n=1, j=psup2[i]+1; j<=psup2[i+1]; ++j) {
      // put in column index of an off-diagonal
      ja[ia[itmp]-1+(n++)] = psup1[j]+1;
    }
  }

  // (bubble-)sort column indices
  for (i=0; i<rnz.size(); ++i)
    for (std::size_t j=psup2[i]+1; j<=psup2[i+1]; ++j)
       for (std::size_t l=1; l<rnz[i]; ++l)   // sort column indices of row i
          for (std::size_t e=0; e<rnz[i]-l; ++e)
            if (ja[ia[i]-1+e] > ja[ia[i]+e])
              std::swap( ja[ia[i]-1+e], ja[ia[i]+e] );

} // Catch std::exception
  catch (std::exception& se) {
    // (re-)throw tk::Excpetion
    std::cerr << std::string("RUNTIME ERROR in CSR constructor: ") + se.what();
    throw;
  }

const double&
CSR::operator()( std::size_t row, std::size_t col ) const
// *****************************************************************************
//  Return const reference to sparse matrix entry
//! \param[in] row Block row
//! \param[in] col Block column
//! \return Const reference to matrix entry
// *****************************************************************************
{
  for (std::size_t n=0, j=ia[row]-1; j<ia[row+1]-1; ++j, ++n)
    if (col+1 == ja[j])
      return a[ia[row]-1+n];

  std::cerr << "Sparse matrix index not found";
  throw;
}

void
CSR::dirichlet( std::size_t i, double val, std::vector< double >& b )
// *****************************************************************************
//  Set Dirichlet boundary condition at a node
//! \param[in] i Local id at which to set Dirichlet BC
//! \param[in] val Value of Dirichlet BC
//! \param[in,out] b RHS to modify as a result of setting the Dirichlet BC
// *****************************************************************************
{
  // apply Dirichlet BC on rhs, zero column
  for (std::size_t r=0; r<rnz.size(); ++r) {
    for (std::size_t j=ia[r]-1; j<ia[r+1]-1; ++j) {
      if (i+1 == ja[j]) {
        b[r] += a[j] * val;
        a[j] = 0.0;
        break;
      }
    }
  }

  // zero row and put in diagonal
  for (std::size_t j=ia[i]-1; j<ia[i+1]-1; ++j) {
    if (i+1 == ja[j]) a[j] = 1.0; else a[j] = 0.0;
  }
}

void
CSR::mult( const std::vector< double >& x, std::vector< double >& r ) const
// *****************************************************************************
//  Multiply CSR matrix with vector from the right: r = A * x
//! \param[in] x Vector to multiply matrix with from the right
//! \param[in] r Result vector of product r = A * x
//! \note This is only complete in serial. In parallel, this computes the own
//!   contributions to the product, so it must be followed by communication
//!   summing the products of rows stored on multiple partitions.
// *****************************************************************************
{
  std::fill( begin(r), end(r), 0.0 );

  for (std::size_t i=0; i<rnz.size(); ++i) {
    for (std::size_t j=ia[i]-1; j<ia[i+1]-1; ++j) {
      r[i] += a[j] * x[ja[j]-1];
    }
  }
}

void
CSR::zero()
// *****************************************************************************
//  Zero matrix values
// *****************************************************************************
{
  std::fill( begin(a), end(a), 0.0 );
}

std::ostream&
CSR::write_stored( std::ostream& os ) const
// *****************************************************************************
//  Write out CSR as stored
//! \param[in,out] os Output stream to write to
//! \return Updated output stream
// *****************************************************************************
{
  os << "size (npoin) = " << rnz.size() << '\n';
  os << "ncomp = 1\n";
  os << "rsize (size) = " << rnz.size() << '\n';
  os << "nnz = " << a.size() << '\n';

  std::size_t i;

  os << "rnz[npoin=" << rnz.size() << "] = { ";
  for (i=0; i<rnz.size()-1; ++i) os << rnz[i] << ", ";
  os << rnz[i] << " }\n";

  os << "ia[rsize+1=" << rnz.size()+1 << "] = { ";
  for (i=0; i<ia.size()-1; ++i) os << ia[i] << ", ";
  os << ia[i] << " }\n";

  os << "ja[nnz=" << ja.size() << "] = { ";
  for (i=0; i<ja.size()-1; ++i) os << ja[i] << ", ";
  os << ja[i] << " }\n";

  os << "a[nnz=" << a.size() << "] = { ";
  for (i=0; i<a.size()-1; ++i) os << a[i] << ", ";
  os << a[i] << " }\n";

  return os;
}

std::ostream&
CSR::write_structure( std::ostream& os ) const
// *****************************************************************************
//  Write out CSR nonzero structure
//! \param[in,out] os Output stream to write to
//! \return Updated output stream
// *****************************************************************************
{
  for (std::size_t i=0; i<rnz.size(); ++i) {
    // leading zeros
    for (std::size_t j=1; j<ja[ia[i]-1]; ++j) os << ". ";
    for (std::size_t n=ia[i]-1; n<ia[i+1]-1; ++n) {
      // zeros between nonzeros
      if (n>ia[i]-1) for (std::size_t j=ja[n-1]; j<ja[n]-1; ++j) os << ". ";
      // nonzero
      os << "o ";
    }
    // trailing zeros
    for (std::size_t j=ja[ia[i+1]-2]; j<rnz.size(); ++j) os << ". ";
    os << '\n';
  }

  return os;
}

std::ostream&
CSR::write_matrix( std::ostream& os ) const
// *****************************************************************************
//  Write out CSR as a real matrix
//! \param[in,out] os Output stream to write to
//! \return Updated output stream
// *****************************************************************************
{
  for (std::size_t i=0; i<rnz.size(); ++i) {
    for (std::size_t j=1; j<ja[ia[i]-1]; ++j) os << "0\t";
    for (std::size_t n=ia[i]-1; n<ia[i+1]-1; ++n) {
      if (n>ia[i]-1) for (std::size_t j=ja[n-1]; j<ja[n]-1; ++j ) os << "0\t";
      os << a[n] << '\t';
    }
    for (std::size_t j=ja[ia[i+1]-2]; j<rnz.size(); ++j) os << "0\t";
    os << '\n';
  }

  return os;
}

std::ostream&
CSR::write_matlab( std::ostream& os ) const
// *****************************************************************************
//  Write out CSR in Matlab/Octave format
//! \param[in,out] os Output stream to write to
//! \return Updated output stream
// *****************************************************************************
{
  os << "A = [ ";
  for (std::size_t i=0; i<rnz.size(); ++i) {
    for (std::size_t j=1; j<ja[ia[i]-1]; ++j) os << "0 ";
    for ( std::size_t n=ia[i]-1; n<ia[i+1]-1; ++n) {
      if (n > ia[i]-1)
        for (std::size_t j=ja[n-1]; j<ja[n]-1; ++j) os << "0 ";
      os << a[n] << ' ';
    }
    for (std::size_t j=ja[ia[i+1]-2]; j<rnz.size(); ++j) os << "0 ";
    os << ";\n";
  }
  os << "]\n";

  return os;
}
