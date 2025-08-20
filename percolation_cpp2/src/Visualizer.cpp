#include "Visualizer.h"

#include <filesystem>
#include <random>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

Visualizer::Visualizer(int n_, int gridPixels_, int msDelay_)
    : n(n_),
      gridPixels(gridPixels_),
      // make stats area ~16% of grid height, min 72 px
      statsHeight(std::max(72, gridPixels_ / 6)),
      windowWidth(gridPixels_),
      windowHeight(gridPixels_ + statsHeight),
      delayMs(msDelay_),
      cellSize(static_cast<float>(gridPixels_) / static_cast<float>(n_)),
      window(sf::VideoMode(sf::Vector2u(static_cast<unsigned int>(windowWidth),
                                        static_cast<unsigned int>(windowHeight))),
             "Percolation (SFML)")
{
    window.setFramerateLimit(60);

    // auto-detect font (look in working dir then common macOS paths)
    std::string fontPath = findFontPath();
    if (!fontPath.empty() && font.openFromFile(fontPath)) {
        fontLoaded = true;
        std::cout << "Loaded font: " << fontPath << "\n";
    } else {
        fontLoaded = false;
        std::cerr << "Warning: no font found. Text overlay will be hidden.\n";
    }
}

std::string Visualizer::findFontPath() const {
    // 1) check working directory
    if (std::filesystem::exists("DejaVuSans.ttf")) return "DejaVuSans.ttf";
    if (std::filesystem::exists("Arial.ttf")) return "Arial.ttf";

    // 2) macOS common locations
    const std::string mac1 = "/System/Library/Fonts/Supplemental/Arial.ttf";
    const std::string mac2 = "/Library/Fonts/Arial.ttf";
    const std::string mac3 = "/System/Library/Fonts/SFNS.ttf";

    if (std::filesystem::exists(mac1)) return mac1;
    if (std::filesystem::exists(mac2)) return mac2;
    if (std::filesystem::exists(mac3)) return mac3;

    // not found
    return std::string();
}

sf::Color Visualizer::colorFor(const Percolation& perc, int row, int col) const {
    if (perc.isFull(row, col)) return sf::Color(100, 149, 237); // full -> blue
    if (perc.isOpen(row, col)) return sf::Color::White;        // open -> white
    return sf::Color::Black;                                   // blocked -> black
}
void Visualizer::drawGridAndStats(const Percolation& perc) {
    window.clear(sf::Color(30, 10, 30));

    // Draw grid area (top)
    sf::RectangleShape cell(sf::Vector2f(cellSize - 1.0f, cellSize - 1.0f));
    for (int r = 1; r <= n; ++r) {
        for (int c = 1; c <= n; ++c) {
            float x = (c - 1) * cellSize;
            float y = (r - 1) * cellSize;
            cell.setPosition(sf::Vector2f(x, y));
            cell.setFillColor(colorFor(perc, r, c));
            window.draw(cell);
        }
    }

    // Draw stats background
    sf::RectangleShape statsBg(sf::Vector2f(static_cast<float>(gridPixels), static_cast<float>(statsHeight)));
    statsBg.setPosition(sf::Vector2f(0.f, static_cast<float>(gridPixels)));
    statsBg.setFillColor(sf::Color(40, 40, 40));
    window.draw(statsBg);

    // Draw text into stats area
    if (fontLoaded) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        
        // First line
        ss << "Open: " << perc.numberOfOpenSites() 
           << "    Closed: " << (n * n) - perc.numberOfOpenSites()
           << "    Prob: " << (static_cast<float>(perc.numberOfOpenSites()) / (n * n));
        
        // Second line for status
        if (perc.percolates()) {
            ss << " \n ";
            ss << "STATUS: PERCOLATES!";
        }

        sf::Text infoText(font, sf::String(ss.str()), 18u);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(sf::Vector2f(10.f, static_cast<float>(gridPixels) + 10.f));
        window.draw(infoText);
    }

    window.display();
}


void Visualizer::run() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, n);

    Percolation perc(n);

    // openCount tracks open sites (we can read from perc.numberOfOpenSites())
    while (window.isOpen()) {
        // SFML3: pollEvent returns optional
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (!perc.percolates()) {
            // pick a random blocked site
            int r, c;
            do {
                r = dist(gen);
                c = dist(gen);
            } while (perc.isOpen(r, c));

            perc.open(r, c);
            // redraw grid + stats
            drawGridAndStats(perc);
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        } else {
            // final state - still draw final stats
            drawGridAndStats(perc);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
}
