// this is main class (as interface for matrices)
#include <vector>
class matrix
{

public:
    virtual void dot(std::vector<int> connectivity) = 0;        // dot product with connectivity vecotr (raw form)
    virtual void dot(std::vector<std::vector<int>> matrix) = 0; // to multiply with a matrix
    virtual void print() = 0;                                   // print as matrix (vector<vector>)
};
