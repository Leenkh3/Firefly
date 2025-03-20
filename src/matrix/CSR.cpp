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
#include "CSR.h"

CSR::CSR(std::vector<int> &connectivity, int shape_points) {
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

   // Build the CSR structure.
   // Conventionally, the first row pointer is 0.
   this->rows_ptr.clear();
   this->cols.clear();
   int count = 1;
   this->rows_ptr.push_back(count);
   for (std::set<int>::iterator it = unique_elemnts.begin(); 
        it != unique_elemnts.end(); 
        ++it) {
       // Use operator[] instead of .at() so that it works in older compilers.
       std::set<int>& connSet = hash[*it];
       for (std::set<int>::iterator j = connSet.begin(); j != connSet.end(); ++j) {
           this->cols.push_back(*j);
       }
       count += (int)connSet.size();
       this->rows_ptr.push_back(count);
   }
}

 void CSR::reshape(int rows, int cols){};           
 void CSR::T() {};                                   
 std::vector<double> CSR::mult(std::vector<double> &vec)  const{ 
    // I know this is not good - this is just a placeholder for now
    std::vector<double> any;
    return any;

 };           
 void CSR::print() {
    std::cout<<"Printing cols vector" <<std::endl;
    for(int i = 0;i< this->cols.size();i++){
        std::cout<<this->cols[i] << " " ;
    }
    std::cout<<std::endl;
    std::cout<<"Printing row_ptrs vector" <<std::endl;
    for(int i = 0;i< this->rows_ptr.size();i++){
        std::cout<<this->rows_ptr[i] << " " ;

    }
    std::cout<<std::endl;

 };                                 
 double& CSR::at(int row, int col) { 
    // I know this is not good - this is just a placeholder for now
    double test=0.0;
    return test;
 };   
 std::vector<int> CSR::shape() {
    return this->cols;
 };


 //getters
 std::vector<int> CSR::getRPtr(){
    return this->rows_ptr;
 };

 std::vector<int> CSR::getCols(){
    return this->cols;
 }


 bool CSR::equal(std::vector<int> a, std::vector<int> b) const{

    if(a.size()!=b.size()){
        std::cerr<<"You have to compare vectors of same size!!"<<std::endl;
        return false;
    }

    for(int i =0;i<a.size();i++)
        if(a[i]!=b[i]) return false;
    return true;
 }
