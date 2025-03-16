#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
using namespace std;

class Matrix
{
public:
    virtual vector<int> shape() = 0;                           // Get matrix dimensions
    virtual Matrix *reshape(int rows, int cols) = 0;           // Reshape matrix
    virtual Matrix *T() = 0;                                   // Transpose matrix
    virtual Matrix *dot(vector<double> &vec) = 0;              // Dot product with vector
    virtual Matrix *dot(vector<vector<double> > &mat) = 0;     // Dot product with matrix
    virtual void print() = 0;                                  // Print matrix

    virtual ~Matrix() {}                                       // Virtual destructor
};

#endif
