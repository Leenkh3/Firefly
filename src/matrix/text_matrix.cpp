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

    delete &reshaped; // Clean up dynamically allocated memory
    return 0;
}

int test_transpose() {
    std::vector<std::vector<double> > mat(2, std::vector<double>(2, 2.0));
    mat[0][0] = 1.0;
    mat[1][1] = 1.0;

    Dense d(mat);
    Matrix& transposed = d.T();

    std::vector<int> expected_shape;
    expected_shape.push_back(2); // Rows
    expected_shape.push_back(2); // Cols

    if (transposed.shape() != expected_shape) {
        std::cerr << "Transpose test failed!" << std::endl;
        return 1;
    }
    std::cout << "Transpose test passed!" << std::endl;

    delete &transposed; // Clean up dynamically allocated memory
    return 0;
}

int main() {
    int result = 0;

    result |= test_constructor();
    result |= test_reshape();
    result |= test_transpose();

    return result;
}
