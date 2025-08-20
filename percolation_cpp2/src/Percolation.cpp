#include "Percolation.h"
#include <stdexcept>
#include <algorithm>
#include <functional>

Percolation::Percolation(int n_) : n(n_) {
    if (n <= 0) throw std::invalid_argument("n must be > 0");
    int total = n * n;
    virtualTop = 0;
    virtualBottom = total + 1;
    openSites.assign(total + 2, false);
    openCount = 0;

    // Initialize both Union-Find structures
    parent.resize(total + 2);
    sz.assign(total + 2, 1);
    parent_full.resize(total + 1); // indices 0..n*n (virtualTop + all sites), no virtualBottom
    sz_full.assign(total + 1, 1);

    for (int i = 0; i < (int)parent.size(); ++i) parent[i] = i;
    for (int i = 0; i < (int)parent_full.size(); ++i) parent_full[i] = i;
}

int Percolation::index(int row, int col) const {
    return (row - 1) * n + (col - 1) + 1; // range 1..n*n
}

void Percolation::validateRowCol(int row, int col) const {
    if (row < 1 || row > n || col < 1 || col > n) throw std::out_of_range("row/col out of range");
}

int Percolation::findRoot(int x) const {
    if (parent[x] == x) return x;
    return parent[x] = findRoot(parent[x]);
}

void Percolation::unite(int a, int b) {
    int ra = findRoot(a);
    int rb = findRoot(b);
    if (ra == rb) return;
    if (sz[ra] < sz[rb]) std::swap(ra, rb);
    parent[rb] = ra;
    sz[ra] += sz[rb];
}

bool Percolation::connected(int a, int b) const {
    return findRoot(a) == findRoot(b);
}

/* ---------------------
   Fullness-UF (no virtualBottom)
   --------------------- */
int Percolation::findRootFull(int x) const {
    if (parent_full[x] == x) return x;
    return parent_full[x] = findRootFull(parent_full[x]);
}

void Percolation::unite_full(int a, int b) {
    int ra = findRootFull(a);
    int rb = findRootFull(b);
    if (ra == rb) return;
    if (sz_full[ra] < sz_full[rb]) std::swap(ra, rb);
    parent_full[rb] = ra;
    sz_full[ra] += sz_full[rb];
}

bool Percolation::connectedFull(int a, int b) const {
    return findRootFull(a) == findRootFull(b);
}

/* ---------------------
   Core API
   --------------------- */
void Percolation::open(int row, int col) {
    validateRowCol(row, col);
    int id = index(row, col);
    if (openSites[id]) return;
    openSites[id] = true;
    ++openCount;

    // helper to connect to neighbor in both UF structures (where appropriate)
    auto connect_neighbor = [&](int r, int c) {
        if (r >= 1 && r <= n && c >= 1 && c <= n && isOpen(r, c)) {
            int neighbor_id = index(r, c);
            unite(id, neighbor_id);        // main UF (with virtualBottom)
            unite_full(id, neighbor_id);   // fullness UF (no virtualBottom)
        }
    };

    // If in top row, connect to virtualTop in both structures
    if (row == 1) {
        unite(id, virtualTop);
        unite_full(id, virtualTop);
    }

    // If in bottom row, connect to virtualBottom ONLY in main UF (to avoid backwash)
    if (row == n) {
        unite(id, virtualBottom);
    }

    // Connect to all four neighbors
    connect_neighbor(row - 1, col);
    connect_neighbor(row + 1, col);
    connect_neighbor(row, col - 1);
    connect_neighbor(row, col + 1);
}

bool Percolation::isOpen(int row, int col) const {
    validateRowCol(row, col);
    return openSites[index(row, col)];
}

bool Percolation::isFull(int row, int col) const {
    validateRowCol(row, col);
    if (!isOpen(row, col)) return false;

    // Use the SECOND Union-Find structure (parent_full) which does NOT have virtualBottom
    return connectedFull(virtualTop, index(row, col));
}

bool Percolation::percolates() const {
    return connected(virtualTop, virtualBottom);
}

int Percolation::numberOfOpenSites() const {
    return openCount;
}

/* ---------------------
   wouldPercolateIfOpened
   simulate on a local copy of main UF to test hypothetical opening
   --------------------- */
bool Percolation::wouldPercolateIfOpened(int row, int col) const {
    validateRowCol(row, col);
    int id = index(row, col);
    if (openSites[id]) return false; // already open — not a "new" opening

    // make copies of parent and size for simulation (main UF includes virtualBottom)
    std::vector<int> p = parent;
    std::vector<int> s = sz;

    // local find with path compression on copy p
    std::function<int(int)> findLocal = [&](int x) -> int {
        while (p[x] != x) {
            p[x] = p[p[x]];
            x = p[x];
        }
        return x;
    };
    auto uniteLocal = [&](int a, int b) {
        a = findLocal(a); b = findLocal(b);
        if (a == b) return;
        if (s[a] < s[b]) std::swap(a, b);
        p[b] = a;
        s[a] += s[b];
    };

    // connect id to virtual top/bottom if on first/last row in simulation
    if (row == 1) uniteLocal(id, virtualTop);
    if (row == n) uniteLocal(id, virtualBottom);

    // connect to currently open neighbors (use openSites)
    auto tryNeighbor = [&](int r2, int c2) {
        if (r2 < 1 || r2 > n || c2 < 1 || c2 > n) return;
        int nid = index(r2, c2);
        if (openSites[nid]) uniteLocal(id, nid);
    };

    tryNeighbor(row - 1, col);
    tryNeighbor(row + 1, col);
    tryNeighbor(row, col - 1);
    tryNeighbor(row, col + 1);

    // check if virtual top and bottom connected in the simulated union-find
    return findLocal(virtualTop) == findLocal(virtualBottom);
}
