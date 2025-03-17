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

    Matrix& reshaped = d.reshape(4, 1);

    std::vector<int> expected_shape;
    expected_shape.push_back(4); // Rows
    expected_shape.push_back(1); // Cols

    if (reshaped.shape() != expected_shape) {
        std::cerr << "Reshape test failed!" << std::endl;
        return 1;
    }
    std::cout << "Reshape test passed!" << std::endl;

    return 0;
}


int test_not_valid_reshape() {
    std::vector<std::vector<double> > mat(2, std::vector<double>(2, 2.0));
    Dense d(mat);

    Matrix& reshaped = d.reshape(4, 4);

    std::vector<int> expected_shape;
    expected_shape.push_back(2); // Rows
    expected_shape.push_back(2); // Cols

    if (reshaped.shape() != expected_shape) {
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
    Matrix& transposed = d.T();

    std::vector<int> expected_shape;
    expected_shape.push_back(2); // Rows
    expected_shape.push_back(2); // Cols

    if ((transposed.shape() != expected_shape) || (mat[1][0] != 4.0 && mat[0][1] != 2.0) ) {
        std::cerr << "Transpose test failed!" << std::endl;
        return 1;
    }

    
    std::cout << "Transpose test passed!" << std::endl;

    return 0;
}


int test_transpose_3x2_array() {
    // Build a 3x2 matrix using push_back for C++ compatibility.
    std::vector<std::vector<double> > mat;
    
    {
        std::vector<double> row;
        row.push_back(1.0);
        row.push_back(2.0);
        mat.push_back(row);
    }
    {
        std::vector<double> row;
        row.push_back(3.0);
        row.push_back(4.0);
        mat.push_back(row);
    }
    {
        std::vector<double> row;
        row.push_back(5.0);
        row.push_back(6.0);
        mat.push_back(row);
    }
    
    Dense d(mat);
    Matrix &transposed = d.T();
    
    std::vector<int> expected_shape;
    expected_shape.push_back(2);  // Rows after transpose.
    expected_shape.push_back(3);  // Columns after transpose.
    
    if (transposed.shape() != expected_shape) {
        std::cerr << "Transpose test failed: Incorrect shape. Expected [2, 3] but got [";
        std::vector<int> shape = transposed.shape();
        for (size_t i = 0; i < shape.size(); ++i) {
            std::cerr << shape[i];
            if (i + 1 < shape.size()) {
                std::cerr << ", ";
            }
        }
        std::cerr << "]." << std::endl;
        return 1;
    }
    
    if (transposed.at(0,0) != 1.0 ||
        transposed.at(0,1) != 3.0 ||
        transposed.at(0,2) != 5.0 ||
        transposed.at(1,0) != 2.0 ||
        transposed.at(1,1) != 4.0 ||
        transposed.at(1,2) != 6.0)
    {
        std::cerr << "Transpose test failed: Incorrect values in the transposed matrix." << std::endl;
        return 1;
    }
    
    std::cout << "Transpose test passed!" << std::endl;
    return 0;
}

int main() {
    int result = 0;

    result |= test_constructor();
    result |= test_reshape();
    result |= test_not_valid_reshape();
    result |= test_transpose();
    result |= test_transpose_3x2_array();

    return 0;
}
