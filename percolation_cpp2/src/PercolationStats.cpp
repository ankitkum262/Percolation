#include "PercolationStats.h"
#include "Percolation.h"
#include <random>
#include <numeric>
#include <cmath>
#include <iostream>
#include <stdexcept>

PercolationStats::PercolationStats(int n_, int trials_) : n(n_), trials(trials_) {
    if (n <= 0) throw std::invalid_argument("n must be > 0");
    if (trials <= 0) throw std::invalid_argument("trials must be > 0");
    thresholds.reserve(trials);
}

void PercolationStats::runExperiments() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, n);

    for (int t = 0; t < trials; ++t) {
        Percolation perc(n);
        while (!perc.percolates()) {
            int r = dist(gen);
            int c = dist(gen);
            if (!perc.isOpen(r, c)) {
                perc.open(r, c);
            }
        }
        thresholds.push_back(static_cast<double>(perc.numberOfOpenSites()) / (n * n));
    }
}

double PercolationStats::mean() const {
    if (thresholds.empty()) return 0.0;
    double s = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
    return s / thresholds.size();
}

double PercolationStats::stddev() const {
    if (thresholds.size() <= 1) return 0.0;
    double m = mean();
    double acc = 0.0;
    for (double x : thresholds) {
        double d = x - m;
        acc += d * d;
    }
    return std::sqrt(acc / (thresholds.size() - 1));
}

std::pair<double,double> PercolationStats::confidence95() const {
    double m = mean();
    double s = stddev();
    double t = static_cast<double>(thresholds.size());
    if (t == 0) return {0.0, 0.0};
    double margin = 1.96 * s / std::sqrt(t);
    return {m - margin, m + margin};
}

void PercolationStats::printResults() const {
    auto [lo, hi] = confidence95();
    std::cout << "mean                    = " << mean() << "\n";
    std::cout << "stddev                  = " << stddev() << "\n";
    std::cout << "95% confidence interval = [" << lo << ", " << hi << "]\n";
}
