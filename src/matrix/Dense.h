#ifndef DENSE_H
#define DENSE_H

#include "Matrix.h"
#include <iostream>
using namespace std;

class Dense : public Matrix
{
private:
    vector<vector<double> > matrix;
    vector<int> matrix_shape;

    void set_shape(int rows,int cols);
    void set_matrix(vector<vector<double> > matrix);

public:
    Dense(const vector<vector<double> > &mat);
    vector<int> shape() ;
    void print() ;
    Matrix *reshape(int rows, int cols);
    Matrix *T();
    Matrix *dot( vector<double> &vec);
    Matrix *dot( vector<vector<double> > &mat) ;
};

#endif