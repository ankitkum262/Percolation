#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "Percolation.h"
#include <SFML/Graphics.hpp>
#include <string>

class Visualizer {
public:
    // n_            : grid size N (N x N)
    // gridPixels_   : pixel size of the grid area (width and grid height)
    // msDelay_      : delay between opens in milliseconds
    explicit Visualizer(int n_, int gridPixels_ = 720, int msDelay_ = 40);
    void run();

private:
    int n;
    int gridPixels;       // pixels for the square grid area
    int statsHeight;      // height in pixels reserved below the grid for stats
    int windowWidth;
    int windowHeight;
    int delayMs;
    float cellSize;       // pixel size per cell (float)

    int finalCriticalCells = -1;
    double finalNextStepProbability = -1.0;

    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded = false;

    // draw helpers
    sf::Color colorFor(const Percolation& perc, int row, int col) const;
    void drawGridAndStats(const Percolation& perc);

    // font auto-detect helper
    std::string findFontPath() const;
};

#endif
