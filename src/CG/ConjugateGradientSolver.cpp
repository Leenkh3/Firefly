#include <iostream>
#include <vector>
#include <cmath>
#include "CGMatrix.h"

using namespace std;

int main() {
    int N = 2;
    vector<vector<double>> A = {{4, 1}, {1, 3}};
    vector<double> b = {1, 2};
    vector<double> x(N, 0.0);

    conjugateGradient(A, b, x);

    cout << "Firefly Project" << endl;
    cout << "Solution x: ";
    for (size_t i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }
    cout << endl;

    return 0;
}
