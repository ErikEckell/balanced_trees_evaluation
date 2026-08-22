# Balanced trees

C++ implementations of three classic binary search tree structures, each in its own self-contained CMake project:

- **[abb/](abb/)** — Binary Search Tree (BST)
- **[avl/](avl/)** — AVL self-balancing tree
- **[rb_tree/](rb_tree/)** — Red-Black self-balancing tree

Each variant provides `insert`, `find`, and a `traverse` pretty-printer, and ships with a small `main` that benchmarks bulk insertion and search using binary key files.

## Project layout

```
abb/
  include/trees/abb.hpp, abbNode.hpp   # ABB and ABBNode class declarations
  src/abb.cpp, abbNode.cpp             # implementations
  tests/test.cpp                       # benchmark driver (main)
  CMakeLists.txt                       # builds the "test_abb" executable

avl/
  include/trees/avl.hpp, avlNode.hpp   # AVL and AVLNode class declarations
  src/avl.cpp, avlNode.cpp             # implementations
  tests/test.cpp                       # benchmark driver (main)
  CMakeLists.txt                       # builds the "test_avl" executable

rb_tree/
  include/trees/rb.hpp, rbNode.hpp     # RB and RBNode class declarations
  src/rb.cpp, rbNode.cpp               # implementations
  test.cpp                             # benchmark driver (main)
  keys.bin, keys_sorted.bin            # sample binary key datasets
  CMakeLists.txt                       # builds the "test" executable
```

## What each tree implements

### ABB — Binary Search Tree (`trees::ABB` / `trees::ABBNode`)

A plain, unbalanced BST.

- `insert(val)` / `insert_rec(val, node)` — recursive insertion (duplicates go right).
- `find(val)` / `find_rec(val, node)` — recursive search.
- `findnotrec(val)` — iterative (non-recursive) search.
- `traverse()` / `traverse_rec(node, level)` — indented pre-order dump of the tree, printing each node's value and subtree size.
- `showASC()` / `showASC_rec(node)` — in-order traversal, printing keys in ascending order.
- `updateSize()` / `updateSize_rec(node)` — recomputes and caches each node's subtree size (`1 + left size + right size`).
- `k_element(k)` / `k_element_rec(k, node)` — order-statistic query: returns the node holding the k-th smallest key, using the cached subtree sizes (requires `updateSize()` to have been called first).

### AVL — Self-balancing Binary Search Tree (`trees::AVL` / `trees::AVLNode`)

A height-balanced BST that rebalances itself after every insertion.

- Each `AVLNode` tracks its parent, left/right subtree heights, and whether it is a left or right child (`NodeType`).
- `insert(val)` / `insert_rec(val, node)` — recursive insertion that updates subtree heights on the way back up and triggers `balance()` whenever a node's balance score (`|leftHeight - rightHeight|`) exceeds 1.
- `find(val)` / `find_rec(val, node)` — recursive search.
- `traverse()` / `traverse_rec(node, level)` — indented dump showing each node's value and whether it is a left (`L`) or right (`R`) child.
- Rebalancing is implemented via the four classic rotations, dispatched by `getRotationType()`:
  - `leftRotation` / `rightRotation` — single rotations (right-right / left-left cases).
  - `leftRightRotation` / `rightLeftRotation` — double rotations (left-right / right-left cases).

### Red-Black Tree — Self-balancing Binary Search Tree (`trees::RB` / `trees::RBNode`)

A red-black tree that restores red-black invariants after each insertion.

- Each `RBNode` tracks its parent, color (`RED`/`BLACK`), and whether it is a left or right child.
- `insert(val)` / `insert(val, node)` — recursive insertion (new nodes are colored red) that walks back up fixing red-red violations via `balance()`.
- `find(val)` / `find_rec(val, node)` — recursive search.
- `traverse()` / `traverse(node, level)` — indented dump showing each node's value, side (`L`/`R`), and color (`R`/`B`).
- Rebalancing is modeled as six cases (`getRotationType()` → `do_case_1` … `do_case_6`), covering the "node already black", "red uncle (recolor)", and the four red-black rotation cases (left-left, left-right, right-right, right-left), implemented with `rotLeft` / `rotRight`. The root is always forced back to black at the end of `balance()`.
- `keys.bin` and `keys_sorted.bin` are sample binary datasets included alongside this variant for manual experimentation.

## Building

Each tree lives in its own independent CMake project. From the corresponding folder (`abb`, `avl`, or `rb_tree`):

```sh
mkdir build
cd build
cmake ..
make
```

This produces the executable for that variant:

| Folder     | Executable  |
|------------|-------------|
| `abb/`     | `test_abb`  |
| `avl/`     | `test_avl`  |
| `rb_tree/` | `test`      |

Run it from the `build` directory, e.g. `./test_abb`, `./test_avl`, or `./test`.

Requirements: CMake ≥ 3.10 and a C++11-capable compiler (the `CMakeLists.txt` files default `CMAKE_CXX_COMPILER` to `/usr/bin/g++`, so on Linux/WSL with g++ installed this works out of the box; on other setups pass `-DCMAKE_CXX_COMPILER=<your g++/clang++ path>` to `cmake`).

### ⚠️ Known caveats

- All `#include` directives (in headers, sources, and the test drivers) use **hardcoded absolute paths** of the form `/home/edauandes/eda_cpp/trees_tarea3/<tree>/include/trees/...`. To build on a different machine or path, update these includes to relative paths (e.g. `"trees/abb.hpp"`, relying on the `target_include_directories` already set in each `CMakeLists.txt`) or adjust them to match your local checkout path.
- Each `main` (in `tests/test.cpp` / `rb_tree/test.cpp`) loads its benchmark input from hardcoded absolute paths under `/home/edauandes/eda_cpp/data_trees/` (e.g. `keys_sorted_32768.bin`, `keys_32768.bin`, `keys_1048576.bin`, `queries_1000.bin`), which are **not included in this repository**. Supply your own binary key files (each a flat sequence of little-endian 4-byte `int` values) at those paths, or edit the paths in the respective `test.cpp` to point at your own datasets — `rb_tree/keys.bin` and `rb_tree/keys_sorted.bin` are provided as sample data you can adapt for this purpose.

## Benchmark driver

Each `test.cpp` follows the same pattern:

1. Reads a dataset of keys and a set of 1000 query keys from binary files (`readKeysFromFile`, which parses raw 4-byte integers).
2. Bulk-inserts all keys into the tree, timing the construction with `std::chrono::high_resolution_clock`.
3. Runs the 1000 queries against the tree (`find` for AVL/RB, `findnotrec` for ABB), timing the search phase.
4. Prints both durations in nanoseconds to `stdout`.

This is intended to compare insertion/search performance across the three tree structures on the same input sizes.
