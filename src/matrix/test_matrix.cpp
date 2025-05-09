#include "Dense.h"
#include <iostream>
#include <vector>
#include <cassert>
#include "SparseCSR.h"
#include <algorithm>


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



int test_constructor() {
    std::vector<std::vector<double> > mat(2, std::vector<double>(2, 2.0));
    Dense d(mat);

    std::vector<int> expected_shape;
    expected_shape.push_back(2); // Rows
    expected_shape.push_back(2); // Cols

    if (d.shape() != expected_shape) {
        std::cerr << "Constructor test failed!" << std::endl;
        return 1;
    }
    std::cout << "Constructor test passed!" << std::endl;
    return 0;
}

int test_reshape() {
    std::vector<std::vector<double> > mat(2, std::vector<double>(2, 2.0));
    Dense d(mat);

    d.reshape(4, 1);

    std::vector<int> expected_shape;
    expected_shape.push_back(4); // Rows
    expected_shape.push_back(1); // Cols

    if (d.shape() != expected_shape) {
        std::cerr << "Reshape test failed!" << std::endl;
        return 1;
    }
    std::cout << "Reshape test passed!" << std::endl;

    return 0;
}


int test_not_valid_reshape() {
    std::vector<std::vector<double> > mat(2, std::vector<double>(2, 2.0));
    Dense d(mat);

    d.reshape(4, 4);

    std::vector<int> expected_shape;
    expected_shape.push_back(2); // Rows
    expected_shape.push_back(2); // Cols

    if (d.shape() != expected_shape) {
        std::cerr << "Reshape test failed!" << std::endl;
        return 1;
    }
    std::cout << "Reshape test passed!" << std::endl;

    return 0;
}



int test_transpose() {
    std::vector<std::vector<double> > mat(2, std::vector<double>(2, 2.0));
    mat[0][0] = 3.0;
    mat[0][1]= 4.0;
    mat[1][1] = 5.0;

    Dense d(mat);
    d.T();

   
    if (d.at(1,0) != 4.0 || d.at(0,1)!= 2.0 ) {
        std::cerr << "Transpose test failed! ,matrix is :" << std::endl;
        return 1;
    }

    
    std::cout << "Transpose test passed!" << std::endl;

    return 0;
}


int test_transpose_3x2_array() {
    // Build a 3x2 matrix using push_back for C++ compatibility.
    std::vector<std::vector<double> > mat;
    
        std::vector<double> row;
        row.push_back(1.0);
        row.push_back(2.0);
        mat.push_back(row);
        row.clear();
        row.push_back(3.0);
        row.push_back(4.0);
        mat.push_back(row);
        row.clear();
        row.push_back(5.0);
        row.push_back(6.0);
        mat.push_back(row);
    
    Dense d(mat);
    d.T();
    
    std::vector<int> expected_shape;
    expected_shape.push_back(2);  // Rows after transpose.
    expected_shape.push_back(3);  // Columns after transpose.
    
    if (d.shape() != expected_shape) {
        std::vector<int> shape = d.shape();
        std::cerr << "Transpose test failed: Incorrect shape. Expected [2, 3] but got ["<<shape[0]<<","<<shape[1]<<"]"<<std::endl;
        return 1;
    }
    
    if (d.at(0,0) != 1.0 ||
        d.at(0,1) != 3.0 ||
        d.at(0,2) != 5.0 ||
        d.at(1,0) != 2.0 ||
        d.at(1,1) != 4.0 ||
        d.at(1,2) != 6.0)
    {
        std::cerr << "Transpose test failed: Incorrect values in the transposed matrix." << std::endl;
        return 1;
    }
    
    std::cout << "Transpose 3*2 test passed!" << std::endl;
    return 0;
}


int test_getter_and_setter(){
    int row = 0, col = 0;
    std::vector<std::vector<double> >mat(2,std::vector<double>(2,2.0));
    Dense d(mat);

    d.at(0,0)=1.0;
    if(d.at(0,0) != 1.0){
        std::cerr << "Setter/Getter test failed: Incorrect value in the requested matrix location." << std::endl;
        return 1;
    }
    std::cout << "Setter/Getter test passed!" << std::endl;
    return 0;
}



int test_vector_multiplication(){

    std::vector<std::vector<double> > mat(3,std::vector<double>(2,2.0));

    Dense d(mat);
    // original array
    // 1.0 2.0
    // 3.0 4.0
    // 2.0 2.0
    d.at(0,0) = 1.0;
    d.at(1,0) = 3.0;
    d.at(1,1) = 4.0;

    // multiply with vector of shape (2,1) ={1,2} (row vector)
    std::vector<double> to_mult(2,2.0);
    to_mult[0] = 1.0;

    // the result must be of shape (3,1) {5.0,11.0,6.0} as row matrix 
    std::vector<double> results = d.mult(to_mult);
    // check the shape

    std::vector<int> shape = d.shape();

    if(shape[0] != 3 && shape[1] != 2){
        std::cerr<<"Matrix Multiplication Test Faild : shape after multiplication must be (3,1) , but it is(" <<shape[0]<<","<<shape[1]<<")!"<<std::endl;
        return 1;
    }

    if(results[0] != 5.0 || results[1] != 11.0 || results[2]!=6.0){
        std::cerr<<"Matrix Multiplication Test Faild :Wrong matrix values, the resulted (wrong matrix) is: !!"<<std::endl;
        return 1;
    }

    std::cout<<"Multiplication with vector test passed!"<<std::endl;
    return 0;


}




int test_SparseCSR_format(){
    int connectivity_arr[] ={
        1,2,4,5,
        2,3,5,6,
        4,5,7,8,
        5,6,8,9
    };

    int count = sizeof(connectivity_arr) / sizeof(connectivity_arr[0]);
    
     std::vector<std::size_t> connectivity(connectivity_arr,connectivity_arr+count);

    shiftToZero(connectivity);

    SparseCSR s(connectivity,4);
    


    // for cols it must be as follows
    std::size_t cols_arr[] = {
        1, 2, 4, 5,
        1, 2, 3, 4, 5, 6,
        2, 3, 5, 6,
        1, 2, 4, 5, 7, 8,
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        2, 3, 5, 6, 8, 9,
        4, 5, 7, 8,
        4, 5, 6, 7, 8, 9,
        5, 6, 8, 9
    };
    
    count = sizeof(cols_arr) / sizeof(cols_arr[0]);
    
    // Create a std::vector using the cols_array.
    std::vector<int> cols(cols_arr, cols_arr + count);

    int row_ptrs_arr[]={1,5,11,15,21,30,36,40,46,50};
    count = sizeof(row_ptrs_arr) / sizeof(row_ptrs_arr[0]);
    std::vector<int> rows_ptr(row_ptrs_arr,row_ptrs_arr + count);

    if(cols == s.getCols() && rows_ptr == s.getRPtr()){
        std::cout<<"SparseCSR test passed!"<<std::endl;
        return 0;
    }


    std::cerr<<"Wrong cols|row_ptr vector!!" <<std::endl;
    return 1;
}

int SparseCSR_2(){
    int connectivity_arr[]= { 12, 14,  9, 11,
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
    int count = sizeof(connectivity_arr)/sizeof(connectivity_arr[0]);
    std::vector<std::size_t> connectivity(connectivity_arr,connectivity_arr+count);

    shiftToZero(connectivity);
    SparseCSR s(connectivity,4);

    int row_ptrs_arr[] = { 1, 8, 15, 22, 29, 36, 43, 50, 57, 66, 75, 84, 94, 103, 113 };
    count = sizeof(row_ptrs_arr)/sizeof(row_ptrs_arr[0]);
    std::vector<int> rows_ptr(row_ptrs_arr,row_ptrs_arr+count);

    int cols_arr[]={ 1, 2, 4, 5, 9, 11, 14, 1, 2, 3, 6, 9, 11, 12, 2, 3, 4, 7, 9, 12, 13, 1, 3, 4, 8, 9, 13, 14, 1, 5, 6, 8, 10, 11, 14, 2, 5, 6, 7, 10, 11, 12, 3, 6, 7, 8, 10, 12, 13, 4, 5, 7, 8, 10, 13, 14, 1, 2, 3, 4, 9, 11, 12, 13, 14, 5, 6, 7, 8, 10, 11, 12, 13, 14, 1, 2, 5, 6, 9, 10, 11, 12, 14, 2, 3, 6, 7, 9, 10, 11, 12, 13, 14, 3, 4, 7, 8, 9, 10, 12, 13, 14, 1, 4, 5, 8, 9, 10, 11, 12, 13, 14 };
    count = sizeof(cols_arr)/sizeof(cols_arr[0]);
    std::vector<int> cols(cols_arr,cols_arr+count);


    if(cols ==s.getCols() && rows_ptr == s.getRPtr()){
        
        std::cout<<"SparseCSR  test 2 passed!"<<std::endl;
        return 0;       
    }

    std::cerr<<"Wrong cols|row_ptr vector!!" <<std::endl;
        return 1;
}
//! [param] return_value : boolean : if true it will return the actual value
//! else it will return the value of the test (0 if passed, 1 if not) 
int test_sparse_getter(){
    
    int connectivity_arr[] ={
        1,2,4,5,
        2,3,5,6,
        4,5,7,8,
        5,6,8,9
    };

    int count = sizeof(connectivity_arr) / sizeof(connectivity_arr[0]);
    
    const std::vector<std::size_t> connectivity(connectivity_arr,connectivity_arr+count);
    SparseCSR s(connectivity,4);
    
    double val = s.at(4,4); // forth row and 4th column 

    if(val == 0 ) {
        std::cout<<"Getter test for sparse matrix passed" << val <<std::endl;
        return 0;

    }

    std::cerr<<"Getter test for sparse matrix failed! " << val<<std::endl;
    return  1;

}

int test_sparse_setter(){
    int connectivity_arr[] ={
        1,2,4,5,
        2,3,5,6,
        4,5,7,8,
        5,6,8,9
    };

    int count = sizeof(connectivity_arr) / sizeof(connectivity_arr[0]);
    
    const std::vector<std::size_t> connectivity(connectivity_arr,connectivity_arr+count);
    SparseCSR s(connectivity,4);
    
    s.at(0,4)=3.0;

    double actual_value = s.getVals()[2];


    if(actual_value == 3.0){
        std::cout<<"Setter test for sparce matrix passed" << actual_value<<std::endl;
        return 0;
    }
    std::cout<<"Setter test for sparce matrix failed , expected value is 3, got "<< actual_value<<std::endl;
    return 1;
    
}   


int test_CRS_vector_multiplication(){

    int connectivity_arr[] ={
        1,2,4,5,
        2,3,5,6,
        4,5,7,8,
        5,6,8,9
    };

    int count = sizeof(connectivity_arr) / sizeof(connectivity_arr[0]);
    
    const std::vector<std::size_t> connectivity(connectivity_arr,connectivity_arr+count);
    SparseCSR s(connectivity,4);

    double to_mult_arr[] = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0};
    count = sizeof(to_mult_arr) / sizeof(to_mult_arr[0]);
    std::vector<double> to_mult(to_mult_arr,to_mult_arr+count);

    std::vector<double> r = s.mult(to_mult);

    
    std::vector<double> must_equal(to_mult.size(),0);
    if(must_equal == r){
        std::cout<<"SparseCSR-Matrix multiplication with vector test passed"<<std::endl;
        return 0;
    }

    std::cout<<"CRS-matrix multiplication with vector test failed!!"<<std::endl;
    return 1;

}

int test_dirichlet()
{
    std::cout << "test start" << std::endl;
    int connectivity_arr[] = {
        1,2,4,5,
        2,3,5,6,
        4,5,7,8,
        5,6,8,9
    };

    int count = sizeof(connectivity_arr) / sizeof(connectivity_arr[0]);
    std::vector<double> b = {1, 2, 3, 4};

    const std::vector<std::size_t> connectivity(connectivity_arr, connectivity_arr+count);
    SparseCSR s(connectivity, 4);

    s.dirichlet(0, 1, b);

    if (s.getAt(0, 0) == 1.0)
    {
        std::cout << "Dirichlet BC diagonal element not the correct value." << std::endl;
        return 1;
    }

    if (s.getAt(0, 1) != 0.0 || s.getAt(1, 0) != 0.0)
    {
        std::cout << "Dirichlet BC off-diagonal element not the correct value." << std::endl;
        return 1;
    }

    return 0;
}

int main() {
    int result = 0;

    result |= test_constructor();
    result |= test_reshape();
    result |= test_not_valid_reshape();
    result |= test_transpose();
    result |= test_transpose_3x2_array();
    result |= test_getter_and_setter();
    result |= test_vector_multiplication();
    result |= test_SparseCSR_format();
    result |= SparseCSR_2();
    result |= test_sparse_getter();
    result |= test_sparse_setter();
    result |= test_CRS_vector_multiplication();
    result |= test_dirichlet();

    return result;
}
