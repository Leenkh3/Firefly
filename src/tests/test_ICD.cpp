#include <iostream>
#include <vector>
using namespace std;

int main() {
    // Explicitly initialize vector
    vector<vector<double> > A(2, vector<double>(2));  // 2x2 matrix
    A[0][0] = 4; A[0][1] = 1;
    A[1][0] = 1; A[1][1] = 3;

    // Explicitly initialize vector b
    vector<double> b(2);  // 2-element vector
    b[0] = 1;
    b[1] = 2;

    // Traditional for-loop instead of range-based for-loop
    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[i].size(); j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    // Traditional for-loop instead of range-based for-loop for b
    for (size_t i = 0; i < b.size(); i++) {
        cout << b[i] << " ";
    }
    cout << endl;

    return 0;
}

