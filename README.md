# LabWork3: Skip List Implementation in C++ (STL-style)

## Overview
This project implements a **Skip List** data structure in modern C++ (up to C++20 standards) with an interface designed to match STL container conventions. The implementation provides efficient search, insertion, and deletion operations with average logarithmic time complexity while maintaining STL-compatible iterators and memory management.

## Author

Lysenko Leonid 24.B82

## Launch

make

## Running tests

make test

## Make documentation

make docs

## Key Features

### STL Compliance
✔ **Iterator Support**
   - Bidirectional iterators (`begin()`, `end()`, `rbegin()`, `rend()`)
   - Const-correct (`const_iterator`, `cbegin()`, `cend()`)

✔ **Allocator-Aware**
   - Custom memory management via template parameter `Allocator`

✔ **Standard Interface**
   - `insert()`, `erase()`, `find()`, `at()`, `operator[]`
   - Size operations (`size()`, `empty()`)
   - Comparison operators (`==`, `<`, etc.)

### Skip List Specifics
- **Probabilistic Level Generation**
  Configurable promotion probability (`promotion_chance = 0.5` by default)

- **Dynamic Height Adjustment**
  Auto-resizing tower heights up to `max_height` (default: 16)

- **Backward Traversal**
  Each node maintains a `back_link` for reverse iteration

## Implementation Highlights

### Core Structure
```cpp
struct SkipNode {
    KeyType node_key;
    ValueType node_value;
    SkipNode* back_link;
    std::vector<SkipNode*> forward_links;  // Tower of forward pointers
};
