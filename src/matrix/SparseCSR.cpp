/*
    Symmetric Matrix Utilization:
    Connectivity points represent shapes connected in space, yielding a symmetric matrix.
    Storing only the upper triangular part saves both memory and computation time.

    Row and Column Vectors:
    From the upper triangular matrix, row_ptr and cols vectors are generated.
    Each row's non-zero elements are counted to construct row_ptr.

    Hash Table Usage:
    A hash table stores intermediate connectivity information to expedite further iterations.
    For example:
    If point 0 connects to point 8, the hash table stores: [8]: [0,...].
    Later, when processing row 8, pre-saved connections are accessed for efficient computation.
    
    Example: let's consider this symmetrix matrix
    1  1  0  0  0
    1  1  1  0  0
    0  1  1  1  0
    0  0  1  1  1
    0  0  0  1  1

    the upper triangular is :
    1  1  -  -  -  
       1  1  -  -  
          1  1  -  
             1  1  
                1
    the hash table first will contain [1]:[0]
    when reaching the iteration (i=1) I will check the hash on i=1 -> there is connectivity with point 0 --- and so on ...

*/

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include "SparseCSR.h"
#include <cassert>


SparseCSR::SparseCSR(const std::vector<std::size_t> &connectivity, int shape_points) {
   /*!
   * \brief Constructs a SparseCSR object from mesh connectivity data.
   *
   * This constructor builds a compressed sparse row (CSR) representation
   * of a mesh's connectivity structure.
   *
   * The connectivity data defines how shape nodes are connected in space,
   * resulting in a symmetric matrix. In order to optimize memory usage and
   * computational effort, only the upper triangular portion of the matrix
   * is stored. From this upper triangular matrix, two vectors are created:
   *
   * - **row_ptr**: This vector records the index in the \c cols vector where
   *   each row's nonzero elements begin.
   * - **cols**: This vector holds the column indices corresponding to the
   *   nonzero entries.
   *
   * Additionally, a hash table is employed to cache intermediate connectivity
   * information, which accelerates subsequent computations. For example, if point
   * 0 connects to point 8, the hash table stores an association such as:
   *   \code
   *   [8] : [0, ...]
   *   \endcode
   * Later, when processing row 8, the algorithm retrieves the pre-saved connectivity
   * from the hash table for efficient computation.
   *
   * \param[in] connectivity A vector that encodes the connectivity of the mesh.
   *                         Its organization produces a symmetric matrix.
   * \param[in] shape_points The number of points per element, which defines
   *                         the overall structure of the mesh.
   *
   * \note Example:
   * Consider the symmetric matrix:
   *
   * \verbatim
   *   1  1  0  0  0
   *   1  1  1  0  0
   *   0  1  1  1  0
   *   0  0  1  1  1
   *   0  0  0  1  1
   * \endverbatim
   *
   * Its stored upper triangular part is:
   *
   * \verbatim
   *   1  1  -  -  -
   *      1  1  -  -
   *         1  1  -
   *            1  1
   *               1
   * \endverbatim
   *
   * Initially, the hash table will, for instance, store an entry like [1] : [0].
   * When processing row \(i = 1\), the constructor consults the hash table to
   * quickly access pre-saved connectivity data, and so on.
   */

   assert (connectivity.size() % shape_points == 0) ;
       

   // Use std::map and std::set for our connectivity hash table.
   std::map<int, std::set<int> > hash;
   int curr;    // current element
   int curr_group, prev_group; // group variables for processing (explanation TBD)

   // Construct a sorted set of unique connectivity points.
   std::set<int> unique_elemnts(connectivity.begin(), connectivity.end());

   /* In case connectivity vector is not sorted :
      EX: 2 3 5 6 , 1 2 4 5 , 4 5 7 8 , 5 6 8 9
      the first element of the matrix is 1, however in this case we will check 2 as first value
      (which is not good). One solution is to sort groups after sorting elements in the group OR
      we can save the upper triangular part in a hash table (nonzero) and later construct cols, rows,
      and values.
   */

   for (int i = 0; i < (int)connectivity.size(); i++) {
       // Initialize prev_group for this outer loop iteration.
       prev_group = -1;
       curr = connectivity[i];
       for (int j = i; j < (int)connectivity.size(); j++) {
           curr_group = j / shape_points;  // using the passed parameter shape_points
           if (connectivity[j] != curr || curr_group == prev_group)
               continue;
           prev_group = curr_group; // Process this group only once.
           j = curr_group * shape_points;  // Roll j back to the beginning of the group.
           // For all points in the current group, record symmetric connectivity.
           for (int k = 0; k < shape_points; k++) {
               hash[curr].insert(connectivity[j + k]);
               hash[connectivity[j + k]].insert(curr);
           }
       }
       prev_group = i / shape_points;
   }

   // Build the SparseCSR structure.
   // Conventionally, the first row pointer is 0.
   rows_ptr.clear();
   cols.clear();
   vals.clear();
   int count = 1;
   rows_ptr.push_back(count);
   for (std::set<int>::iterator it = unique_elemnts.begin(); 
        it != unique_elemnts.end(); 
        ++it) {
       // Use operator[] instead of .at() so that it works in older compilers.
       std::set<int>& connSet = hash[*it];
       for (std::set<int>::iterator j = connSet.begin(); j != connSet.end(); ++j) {
           cols.push_back(*j +  1);
           vals.push_back(0); // all elements values are initialized as 0
       }
       count += (int)connSet.size();
       rows_ptr.push_back(count);
   }


   hash.clear(); //free up map space 
}

 void SparseCSR::reshape(int rows, int cols){
    std::cout<<"Sorry , this functionality is not available in Sparse matrices!";
 };           
 void SparseCSR::T() {
    std::cout<<"Sorry , this functionality is not available in Sparse matrices!";
 };    
 
 
 std::vector<double> SparseCSR::mult(std::vector<double> &x)  const{ 
// *****************************************************************************
//  Multiply CSR matrix with vector from the right: r = A * x
//! \param[in] x Vector to multiply matrix with from the right
// *****************************************************************************
    if(x.size() != rows_ptr.size()-1){
      std::cerr<<"Cannot multiply matrix by vector : Wrong shapes" << x.size() << " != "<< rows_ptr.size()<<std::endl;
      throw;
    }
   
   std::vector<double> r;
   double sum=0.0;
   int i = 1,j=0;
   while(i < rows_ptr.size() && j < cols.size()){
    
      sum+=vals[j] * x[cols[j] - 1];
      j++;
      if(j == rows_ptr[i] -1){ 
         i++;
         r.push_back(sum);
         sum =0;
         
      }
   }

    return r;

 };           
 void SparseCSR::print() const {

// *****************************************************************************
//! \short Write out CSR as stored
// *****************************************************************************
   std::size_t i;

  std::cout << "rows_ptr[rsize+1=" << rows_ptr.size() << "] = { ";
  for (i=0; i<rows_ptr.size()-1; ++i) std::cout << rows_ptr[i] << ", ";
  std::cout << rows_ptr[i] << " }\n";

  std::cout << "cols[nnz=" << cols.size() << "] = { ";
  for (i=0; i<cols.size()-1; ++i) std::cout << cols[i] << ", ";
  std::cout << cols[i] << " }\n";

  std::cout << "vals[nnz=" << vals.size() << "] = { ";
  for (i=0; i<vals.size()-1; ++i) std::cout << vals[i] << ", ";
  std::cout << vals[i] << " }\n";

 };     
 
 void SparseCSR::print_matrix() const {
 // *****************************************************************************
//! \short Write out CSR as a real matrix
// *****************************************************************************
   int nrows = rows_ptr.size()-1;
   int ncols = nrows;

   for (int i =0; i < nrows; i++) {
      int start = rows_ptr[i];
      int end   = rows_ptr[i + 1];

      for (int j = 0; j < ncols; j++) {
          double value = 0.0;
          for (int k = start; k < end; k++) {
              if (cols[k - 1] -1 == j) {
                  value = vals[k - 1];
                  break; 
              }
          }
          std::cout << value << " ";
      }
      std::cout << std::endl; 
  }


 };     


 double& SparseCSR::at(int row, int col) { 
    // this function is to get a value based on 0-indexed matrix (first element is 0 not 1)
    for(int j = rows_ptr[row] - 1 ; j < rows_ptr[row+1] -1 ; j++)
        if(cols[j] == col + 1 )  return vals[ j ];
       
   this->print();
    std::cerr<<"Out of Range - Element not found!" << row<< " " << col << " where cols length is" << cols.size();
    throw; 
 };   


 std::vector<int> SparseCSR::shape() {
   std::vector<int> shape;

   // matrix is squared-symmetric (#cols == #rows)
   shape.push_back(rows_ptr.size()-1);
   shape.push_back(rows_ptr.size()-1);

   return shape;
 };


 //getters
 const std::vector<int> &SparseCSR::getRPtr(){
    return rows_ptr;
 };

 const std::vector<int>& SparseCSR::getCols(){
    return cols;
 }

 const std::vector<double>& SparseCSR::getVals(){
    return vals;
 }


 bool operator==(std::vector<int> &a, std::vector<int> &b) {

    if(a.size()!=b.size()){
        std::cerr<<"You have to compare vectors of same size!!"<<std::endl;
        return false;
    }

    for(int i =0;i<a.size();i++)
        if(a[i]!=b[i]) return false;
    return true;
 }



 std::ostream&
SparseCSR::write_matlab( std::ostream& os ) const
// *****************************************************************************
//  Write out CSR in Matlab/Octave format
//! \param[in,out] os Output stream to write to
//! \return Updated output stream
// *****************************************************************************
{
  os << "A = [ ";
  for (std::size_t i=0; i<rows_ptr.size()-1 ; ++i) {
    for (std::size_t j=1; j<cols[rows_ptr[i]-1]; ++j) os << "0 ";
    for ( std::size_t n=rows_ptr[i]-1; n<rows_ptr[i+1]-1; ++n) {
      if (n > rows_ptr[i]-1)
        for (std::size_t j=cols[n-1]; j<cols[n]-1; ++j) os << "0 ";
      os << vals[n] << ' ';
    }
    for (std::size_t j=cols[rows_ptr[i+1]-2]; j<rows_ptr.size()-1 ; ++j) os << "0 ";
    os << ";\n";
  }
  os << "]\n";

  return os;
}
