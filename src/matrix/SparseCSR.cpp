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


SparseCSR::SparseCSR(std::vector<int> &connectivity, int shape_points) {
   // check the validity of connectivity vector
   if (connectivity.size() % shape_points != 0) {
       std::cerr << "Wrong connectivity vector provided! connectivity size:" 
                 << connectivity.size() 
                 << " is not divisible by shape_points:" 
                 << shape_points << std::endl;
       return;
   }

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
           cols.push_back(*j);
           vals.push_back(1); // all elements values are initialized as 1
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
 
 
 std::vector<double> SparseCSR::mult(std::vector<double> &vec)  const{ 

    if(vec.size() != rows_ptr.size()-1){
      std::cerr<<"Cannot multiply matrix by vector : Wrong shapes" << vec.size() << " != "<< rows_ptr.size()<<std::endl;
      throw;
    }
   
   std::vector<double> r;
   double sum=0.0;
   int i = 1,j=0;
   while(i < rows_ptr.size() && j <= cols.size()){
      if(j == rows_ptr[i] -1){ 
         i++;
         r.push_back(sum);
         sum =0;
      }
      sum+=vals[j] * vec[cols[j] - 1];
      j++;
   }

    return r;

 };           
 void SparseCSR::print() const {
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
       {
        if(cols[j] == col + 1) {
            std::cout<<"j is "<< j<<" cols at this point is " << cols[j] <<std::endl ;
            return this->vals[ j ];
        }
       }
    std::cerr<<"Out of Range - Element not found!";
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
