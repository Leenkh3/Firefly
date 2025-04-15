#ifndef CGMATRIX_H
#define CGMATRIX_H

#include <vector>

std::vector<double> matVecMult(const std::vector<std::vector<double>>& A, const std::vector<double>& v);
double dotProduct(const std::vector<double>& v1, const std::vector<double>& v2);
void vectorAdd(std::vector<double>& v1, const std::vector<double>& v2, double alpha);
void vectorSub(std::vector<double>& v1, const std::vector<double>& v2, double alpha);
void conjugateGradient(const std::vector<std::vector<double>>& A, const std::vector<double>& b, std::vector<double>& x);

#endif
