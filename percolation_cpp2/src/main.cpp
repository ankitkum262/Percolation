#include "Visualizer.h"
#include "PercolationStats.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage:\n  ./percolation viz <N>\n";
        std::cout << "       ./percolation stats <N> <trials>\n";
        return 1;
    }
    std::string mode = argv[1];
    int n = std::stoi(argv[2]);

    if (mode == "viz") {
        Visualizer viz(n, 720, 50);
        viz.run();
    } else if (mode == "stats") {
        if (argc < 4) {
            std::cout << "Error: stats mode requires N and trials\n";
            return 1;
        }
        int trials = std::stoi(argv[3]);
        PercolationStats ps(n, trials);
        ps.runExperiments();
        ps.printResults();
    } else {
        std::cout << "Unknown mode\n";
    }
    return 0;
}
