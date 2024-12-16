# TODO

## Bugs & Fixes

- [ ] Try decreasing memory footprint of the Hybrid Trie (make it run on 1GB of system memory)

## Features

- [ ] Hybrid Trie
  - [ ] Write both recursive and non recursive versions of the functions via function pointers and compile time flags
  - [ ] `restrict` pointer arguments on `TrieHybride` parameters
  - [ ] Add rebalancing helper functions and a rebalancing insertion function

## Configuration

- [ ] Add executable builds for major platforms to CI/CD and releases

## Miscellaneous

- [ ] Package/compile as a shared library
- [ ] Write a second executable in C using `graphviz` as a library to visualize tries from their JSON representations
- [ ] Rewrite the report in LaTeX
  - [ ] Add makefile target called "report"

## Done

- [x] Add detailed and updated `make` build instructions to README
- [x] Hybrid Trie
  - [x] Optimize code and remove unnecessary instructions
  - [x] Refactor code for better readability and structure
  - [x] Make sure code is coherent with the values used (NULL, '\0', VALVIDE, newTH(), etc.)
  - [x] Write a print function to visualize the structure on the console
- [x] Find a way to build both debug and release targets in the Makefile
