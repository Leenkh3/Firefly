#pragma once
#include "Matrix.h"
#include <iostream>

class CRS : public Matrix
{
private:
    std::vector<int> cols;
    std::vector<int> rows_ptr;
    std::vector<double> vals;


public:
    CRS(std::vector<int> &connectivity,int shape_points);
    double& at(int row,int col);
    std::vector<int> shape() ;
    void print() ;
    void reshape(int rows, int cols);
    void T();
    void mult( std::vector<double> &vec);
};
