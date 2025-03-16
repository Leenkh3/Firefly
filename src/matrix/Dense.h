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

    void set_shape(int rows,int cols){
        this->matrix_shape.clear();
        this->matrix_shape.push_back(rows);
        this->matrix_shape.push_back(cols);
    }
    void set_matrix(vector<vector<double> > matrix){
        this->matrix = matrix;
    }

public:
    Dense(const vector<vector<double> > &mat) : matrix(mat)
    {
        int rows = mat.size();
        int cols =  mat[0].size();
        matrix_shape.push_back(rows);
        matrix_shape.push_back(cols);
    }

    vector<int> shape()  override
    {
        return matrix_shape;
    }

    void print()  override
    {
    
        for (int i = 0; i < matrix_shape[0]; i++)
        {
            for (int j = 0; j < matrix_shape[1]; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    
    Matrix *reshape(int rows, int cols) override
    {
        if (rows * cols != this->matrix_shape[0] * this->matrix_shape[1])
        {
            cout << "Can't reshape this matrix to " << rows << " rows and " << cols << " cols with shape " <<matrix_shape[0]<<","<<matrix_shape[1] << endl;
            cout << "Aborting" << endl;
            return this;
        }

        vector<vector<double> > reshaped_matrix(rows, vector<double>(cols));
        int curr_row = 0, curr_col = 0;
        
        for (int i = 0; i < this->matrix_shape[0]; i++)
        {
            
            for (int j = 0; j < this->matrix_shape[1]; j++)
            {
                reshaped_matrix[curr_row][curr_col++] = this->matrix[i][j];
                if (curr_col == cols)
                {
                    curr_row++;
                    curr_col = 0;
                }
            }
        }

        this->set_matrix(reshaped_matrix);
        this->set_shape(rows,cols);
        return this;
    }

    Matrix *T() override
    {
        vector<vector<double> > transposed(matrix_shape[1], vector<double>(matrix_shape[0]));

        for (int i = 0; i < matrix_shape[0]; i++)
        {
            for (int j = 0; j < matrix_shape[1]; j++)
            {
                transposed[j][i] = matrix[i][j];
            }
        }

        
        set_matrix(transposed);
        set_shape(matrix_shape[0],matrix_shape[1]);

        return this;
    }

    Matrix *dot( vector<double> &vec) override
    {
        if(vec.size() != matrix_shape[1]) {
            cout<<"Not Matched shapes : Cannot multiply matrix with shape" << matrix_shape[0]<<","<<matrix_shape[1]<< " with vectro of size "<< vec.size() <<endl;
            return this;
        }
        vector<vector<double> > dotted(matrix_shape[0], vector<double>(1, 0));

        
        int sum = 0;
        for(int row = 0;row< matrix_shape[0];row++){
            sum = 0;

            for(int col = 0;col <  matrix_shape[1];col++){
                sum+= vec[col] * matrix[row][col];
            }
            dotted[row][0] = sum;
        }

        Dense* new_dotted = new Dense(dotted);
        return new_dotted;
    }

    Matrix *dot( vector<vector<double> > &mat) override
    {
        cerr << "Matrix dot product not implemented yet" << endl;
        return this;
    }
};

#endif