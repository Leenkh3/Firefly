#pragma once
#include "Matrix.h"
#include <iostream>

class SparseCSR : public Matrix
{
private:
    std::vector<int> cols;
    std::vector<int> rows_ptr;
    std::vector<double> vals;


public:
    SparseCSR(const std::vector<std::size_t> &connectivity,int shape_points);
    double& at(int row,int col);
    std::vector<int> shape() ;
    void print()  const ;
    void print_matrix()  const ;
    void reshape(int rows, int cols);
    void T();
    const std::vector<int> &getRPtr(); //getting rows_ptr vector
    const std::vector<int> &getCols();    // getting cols vector
    const std::vector<double> &getVals();    // getting values vector
    std::vector<double> mult( std::vector<double> &vec) const;
    std::ostream& write_matlab( std::ostream &os ) const;
    friend bool operator==(std::vector<int> &a, std::vector<int> &b) ; // this is a helper function to check if two vectors are equal (values)

};
