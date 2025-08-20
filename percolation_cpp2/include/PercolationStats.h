#pragma once
#include <vector>

class PercolationStats {
public:
    PercolationStats(int n, int trials);
    void runExperiments();
    double mean() const;
    double stddev() const;
    std::pair<double,double> confidence95() const;
    void printResults() const;

private:
    int n;
    int trials;
    std::vector<double> thresholds;
};
