#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"
#include <iostream>

class Dense : public Matrix
{
private:
    std::vector<std::vector<double> > matrix;
    std::vector<int> matrix_shape;

    void set_shape(int rows,int cols);
    void set_matrix(std::vector<std::vector<double> > matrix);

public:
    Dense(const std::vector<std::vector<double> > &mat);
    std::vector<int> shape() ;
    void print() ;
    Matrix &reshape(int rows, int cols);
    Matrix &T();
    Matrix &dot( std::vector<double> &vec);
    Matrix &dot( std::vector<std::vector<double> > &mat) ;
};

#endif