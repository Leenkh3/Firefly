#pragma once
#include "Matrix.h"
#include <iostream>

class CSR : public Matrix
{
private:
    std::vector<int> cols;
    std::vector<int> rows_ptr;
    std::vector<double> vals;


public:
    CSR(std::vector<int> &connectivity,int shape_points);
    double& at(int row,int col);
    std::vector<int> shape() ;
    void print() ;
    void reshape(int rows, int cols);
    void T();
    const std::vector<int> &getRPtr(); //getting rows_ptr vector
    const std::vector<int> &getCols();    // getting cols vector
    std::vector<double> mult( std::vector<double> &vec) const;


    // this is a temporary function - just for testing purposes
    friend bool operator==(std::vector<int> &a, std::vector<int> &b) ; // this is a helper function to check if two vectors are equal (values)

};
