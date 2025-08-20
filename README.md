# Percolation Simulation (C++ with SFML)

This project simulates **percolation** using a grid-based approach and provides:
- A **PercolationStats** module for running Monte Carlo simulations.
- A **Visualizer** that shows the percolation process in real-time using **SFML**.
- Statistical calculations for **mean**, **standard deviation**, and **95% confidence interval**.

---

## Features
- **Dynamic grid visualization** with color-coded open/full sites.
- **Monte Carlo experiments** for percolation threshold estimation.
- Cross-platform build using **CMake**.
- Supports both CLI mode (stats only) and GUI visualization.

---

## Requirements
- **C++17 or newer**
- **SFML 3.x** (or 2.6 if adjusted)
- **CMake** (for building)

---

## Build Instructions
### Linux / macOS
```bash
git clone https://github.com/YOUR_USERNAME/percolation-sfml.git
cd percolation-sfml
mkdir build && cd build
cmake ..
make
# to run the stats
./percolation stats n m   # grid of side n and run the simulation m times
# to run the visualizer
./percolation viz n       # for grid with side n
