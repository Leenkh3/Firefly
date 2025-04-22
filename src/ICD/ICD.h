#ifndef ICD_H
#define ICD_H

#include <vector>

// Forward declaration
std::vector<std::vector<double>> incompleteCholesky(const std::vector<std::vector<double>>& A);
double dotProduct(const std::vector<double>& a, const std::vector<double>& b);
std::vector<double> matVecMult(const std::vector<std::vector<double>>& A, const std::vector<double>& v);

class ICD {
public:
    ICD(const std::vector<std::vector<double>>& A);
    std::vector<double> solve(const std::vector<double>& b) const;
    const std::vector<std::vector<double>>& getL() const { return L; }

private:
    std::vector<std::vector<double>> L;
};

#endif
