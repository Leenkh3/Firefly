#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    // Define a 2x2 matrix A
    vector<vector<double>> A = {
        {4, 1},
        {1, 3}
    };

    // Define a 2x1 vector b
    vector<double> b = {1, 2};

    // Output the matrix A and vector b
    cout << "Matrix A:" << endl;
    for (const auto& row : A) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    cout << "Vector b:" << endl;
    for (double val : b) {
        cout << val << " ";
    }
    cout << endl;

    // Add any testing code you want here (e.g., use your ICD or CGSolver code)

    return 0;
}

