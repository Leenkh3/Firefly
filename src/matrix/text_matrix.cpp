#include "Dense.h"
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

int test_constructor() {
    vector<vector<double> > mat(2, vector<double>(2, 2.0));
    Dense d(mat);

    vector<int> expected_shape;
    expected_shape.push_back(2); // Rows
    expected_shape.push_back(2); // Cols

    if (d.shape() != expected_shape) {
        cerr << "Constructor test failed!" << endl;
        return 1;
    }
    cout << "Constructor test passed!" << endl;
    return 0;
}

int test_reshape() {
    vector<vector<double> > mat(2, vector<double>(2, 2.0));
    Dense d(mat);

    Matrix* reshaped = d.reshape(4, 1);

    vector<int> expected_shape;
    expected_shape.push_back(4); // Rows
    expected_shape.push_back(1); // Cols

    if (reshaped->shape() != expected_shape) {
        cerr << "Reshape test failed!" << endl;
        return 1;
    }
    cout << "Reshape test passed!" << endl;

    delete reshaped; // Clean up dynamically allocated memory
    return 0;
}

int test_transpose() {
    vector<vector<double> > mat(2, vector<double>(2, 2.0));
    mat[0][0] = 1.0;
    mat[1][1] = 1.0;

    Dense d(mat);
    Matrix* transposed = d.T();

    vector<int> expected_shape;
    expected_shape.push_back(2); // Rows
    expected_shape.push_back(2); // Cols

    if (transposed->shape() != expected_shape) {
        cerr << "Transpose test failed!" << endl;
        return 1;
    }
    cout << "Transpose test passed!" << endl;

    delete transposed; // Clean up dynamically allocated memory
    return 0;
}

int main() {
    int result = 0;

    result |= test_constructor();
    result |= test_reshape();
    result |= test_transpose();

    return result;
}
