#include <iostream>
#include <Eigen/Dense> // Main Eigen library header for dense matrix operations

// For random number generation and seeding
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Eigen;

// generate a random symmetric positive-definite matrix
void generateRandomSPDMatrix(MatrixXd &A, int n) {
    srand(time(0));


    MatrixXd M = MatrixXd::Random(n, n);

    // Make it symmetric: A = M + M^T

    A = 0.5 * (M + M.transpose());

    // Ensure it's positive-definite by adding a large value to the diagonal
    A = A + n * MatrixXd::Identity(n, n);
}

// generate a random vector b
void generateRandomVector(VectorXd &b, int n) {
    srand(time(0));
    b = VectorXd::Random(n); // values between -1 and 1
}

int main() {
    int n;
    cout << "Enter the size of the matrix (n): ";
    cin >> n;


    MatrixXd A(n, n); // n x n matrix, Dense matrix declaration
    VectorXd b(n);    // n x 1 vector

    generateRandomSPDMatrix(A, n);

    cout << "matrix A:\n";
    cout << A << endl;


    generateRandomVector(b, n);

    cout << "vector b:\n";
    cout << b.transpose() << endl;

    // Solve the system A * x = b using Cholesky decomposition (LLT)
    VectorXd x = A.llt().solve(b);

    cout << "Solution vector x:\n";
    cout << x.transpose() << endl;

    cout << "Verifying A * x:\n";
 cout << x.transpose() << endl;

    cout << "Verifying A * x:\n";
    cout << (A * x).transpose() << endl;




    return 0;
}
