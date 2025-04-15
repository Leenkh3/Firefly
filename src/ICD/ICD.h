#ifndef ICD_H
#define ICD_H

#include <vector>

class ICD {
public:
    ICD(const std::vector<std::vector<double>>& A);
    std::vector<double> solve(const std::vector<double>& b) const;

private:
    std::vector<std::vector<double>> L;
};

#endif
