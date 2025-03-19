#include "Dense.h"
#include <iostream>
#include <vector>
#include <cassert>


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
        d.print();
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
        d.print();
        return 1;
    }

    std::cout<<"Multiplication with vector test passed!"<<std::endl;
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
    return result;
}
