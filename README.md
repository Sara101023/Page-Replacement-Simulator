# Page Replacement Algorithms Simulator

[![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()

##  Overview

A comprehensive **educational simulator** implementing three classic page replacement algorithms used in Operating Systems memory management:

- **FIFO** (First In First Out)
- **LRU** (Least Recently Used)  
- **CLOCK** (Second Chance)

This tool helps visualize how operating systems decide which pages to evict from memory when space is limited, comparing their performance through page fault rates.

## Features

-  Interactive command-line interface
-  Step-by-step execution trace
-  Visual frame state representation
-  Performance comparison with hit rates
-  Automatic best algorithm detection
-  Cross-platform support (Windows/Linux/macOS)

##  Quick Start

### Prerequisites
- GCC compiler (MinGW on Windows, GCC on Linux/macOS)

### Compilation

```bash
# Clone the repository
git clone https://github.com/yourusername/page-replacement-simulator.git
cd page-replacement-simulator

# Compile with GCC
gcc src/main.c -o simulator.exe   # Windows
gcc src/main.c -o simulator       # Linux/macOS

# Or use the Makefile
make
