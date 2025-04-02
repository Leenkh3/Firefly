#pragma once
#include "Matrix.h"
#include <iostream>

class Dense : public Matrix
{
private:
    std::vector<std::vector<double> > matrix;
    int rows,cols;
    void set_matrix(std::vector<std::vector<double> > matrix);

public:
    Dense(const std::vector<std::vector<double> > &mat);
    double& at(int row,int col);
    std::vector<int> shape() ;
    void print() const;
    void reshape(int rows, int cols);
    void T();
    std::vector<double> mult( std::vector<double> &vec) const;
};
