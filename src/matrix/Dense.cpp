
#include "Dense.h"
#include <iostream>

Dense::Dense(const std::vector<std::vector<double> > &mat) : matrix(mat) {
    //********************************************************************************
    //! \param[in] mat input matrix (2*2 vector)
    //! \brief this will save #rows, #cols and input matrix in the object
    //********************************************************************************

    int rows = mat.size();
    int cols = mat[0].size();
    this->rows = rows;
    this->cols = cols;
    this->matrix = mat;
}



void Dense::set_matrix(std::vector<std::vector<double> > matrix) {
    //********************************************************************************
    //! \param[in] matrix the matrix you want to set in the object
    //! \brief this is a simple setter for the matrix
    //********************************************************************************


    this->matrix = matrix;
    this->rows =matrix.size();
    this->cols = matrix.size() > 0 ? matrix[0].size() : 0;
}

double& Dense::at(int row, int col){
    //********************************************************************************
    //! \param[in] row the wanted element's row position
    //! \param[in] col the wanted element's col position
    //! \brief this works as setter/getter
    //********************************************************************************

    return this->matrix[row][col];
}

// this is just a placeholder for now - it may be deleted in the future
std::vector<int> Dense::shape() {
    //********************************************************************************
    //! \brief to get the shape of the matrix (#rows,#cols)
    //********************************************************************************
    std::vector<int> shape(2);
    shape[0] = rows;
    shape[1] = cols;
    
    return shape;
}

void Dense::print() const{
    //********************************************************************************
    //! \brief to print the matrix 
    //********************************************************************************

    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Dense::reshape(int rows, int cols) {
    //********************************************************************************
    //! \param[in] rows the required rows 
    //! \param[in] cols the required cols
    /*!  
     * \brief this function is used to reshape the array shape to a wanted one
     * for example : if you have a matrix of shape (2,2) you can reshape it to row matrix (4,1)
     * or column matrix (1,4)
     * \note No broadcasting available - if the provided shape is not valid an error will be thrown!    
     */ 
    //********************************************************************************

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
    //********************************************************************************
    /*!
    * \brief Use this if you want to transpose the matrix 
    Example : a.shape() == (3,1) =>a.T() => a.shape() == (1,3)
    */
    //********************************************************************************

    std::vector<std::vector<double> > transposed(cols, std::vector<double>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }
    
    set_matrix(transposed);
}

 std::vector<double> Dense::mult(std::vector<double> &vec) const {
    //********************************************************************************
    //! \param[in] vec A vector to multiply the matrix with (from the right)
    /*!
    * \brief Use this to multiply your dense matrix with a vector
       \return The resulted vector (after multiplication)
    */
    //********************************************************************************
    if (vec.size() != cols) {
        std::cout << "Shapes don't match: cannot multiply matrix with shape " << rows << ","
                  << cols << " with vector of size " << vec.size() << std::endl;
        return vec ;
    }

    std::vector<double>  dotted(rows);
    for (int row = 0; row < rows; row++) {
        double sum = 0;
        for (int col = 0; col < cols; col++) {
            sum += vec[col] * matrix[row][col];
        }
        dotted[row] = sum;
    }
    return dotted;
}

