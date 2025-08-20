#ifndef PERCOLATION_H
#define PERCOLATION_H

#include <vector>

class Percolation {
public:
    explicit Percolation(int n);
    void open(int row, int col);
    bool isOpen(int row, int col) const;
    bool isFull(int row, int col) const;
    bool percolates() const;
    int numberOfOpenSites() const;

    // Returns true if opening the currently blocked site (row,col)
    // would make the system percolate (does NOT modify state).
    bool wouldPercolateIfOpened(int row, int col) const;

private:
    int n;
    std::vector<bool> openSites;    // indexed 0..(n*n+1) where 0 = virtualTop, n*n+1 = virtualBottom
    mutable std::vector<int> parent;        // union-find
    std::vector<int> sz;            // union-find sizes
    int openCount;
    int virtualTop;
    int virtualBottom;

    int index(int row, int col) const; // maps 1-based (row,col) to UF index
    void validateRowCol(int row, int col) const;

    mutable std::vector<int> parent_full;
    std::vector<int> sz_full;

    int findRoot(int x) const;
    void unite(int a, int b);
    bool connected(int a, int b) const;

    // ---- helpers for the 'fullness' UF (no virtualBottom) ----
    int findRootFull(int x) const;
    void unite_full(int a, int b);
    bool connectedFull(int a, int b) const;
};

#endif
