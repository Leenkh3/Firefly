
#include "Dense.h"
#include <iostream>

Dense::Dense(const std::vector<std::vector<double> > &mat) : matrix(mat) {
    int rows = mat.size();
    int cols = mat[0].size();
    this->rows = rows;
    this->cols = cols;
    this->matrix = mat;
}



void Dense::set_matrix(std::vector<std::vector<double> > matrix) {
    this->matrix = matrix;
    this->rows =matrix.size();
    this->cols = matrix.size() > 0 ? matrix[0].size() : 0;
}

double& Dense::at(int row, int col){
    return this->matrix[row][col];
}

// this is just a placeholder for now - it may be deleted in the future
std::vector<int> Dense::shape() {
    std::vector<int> shape(2);
    shape[0] = rows;
    shape[1] = cols;
    
    return shape;
}

void Dense::print() {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Dense::reshape(int rows, int cols) {
    if (rows * cols != this->rows * this->cols) {
        std::cout << "Can't reshape this matrix to " << rows << " rows and " << cols
                  << " cols with shape " << rows << "," << cols << std::endl;
        std::cout << "Aborting" << std::endl;
        return ;
    }

    std::vector<std::vector<double> > reshaped_matrix(rows, std::vector<double>(cols));
    int curr_row = 0, curr_col = 0;

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            reshaped_matrix[curr_row][curr_col++] = this->matrix[i][j];
            if (curr_col == cols) {
                curr_row++;
                curr_col = 0;
            }
        }
    }

    this->set_matrix(reshaped_matrix);
}

void Dense::T() {
    std::vector<std::vector<double> > transposed(cols, std::vector<double>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }
    
    set_matrix(transposed);
}

 void Dense::mult(std::vector<double> &vec) {
    if (vec.size() != cols) {
        std::cout << "Shapes don't match: cannot multiply matrix with shape " << rows << ","
                  << cols << " with vector of size " << vec.size() << std::endl;
        return ;
    }

    std::vector<std::vector<double> > dotted(rows, std::vector<double>(1, 0));
    for (int row = 0; row < rows; row++) {
        double sum = 0;
        for (int col = 0; col < cols; col++) {
            sum += vec[col] * matrix[row][col];
        }
        dotted[row][0] = sum;
    }
    set_matrix(dotted);
}

