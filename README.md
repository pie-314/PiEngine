# PiEngine

[![Milestone](https://img.shields.io/badge/Milestone-1B_Digits-gold.svg)](https://github.com/yourusername/PiEngine)
[![Language](https://img.shields.io/badge/Language-OCaml%20%7C%20C-blue.svg)](#)
[![Algorithm](https://img.shields.io/badge/Algorithm-Chudnovsky-green.svg)](#how-the-algorithm-works)

PiEngine is a high-performance Pi calculator capable of computing billions of digits with extreme precision. It features dual implementations in OCaml and C, serving as a benchmark for high-precision arithmetic and functional vs. systems programming paradigms.

---

## Recent Achievement: 1,000,000,000 Digits
We have successfully reached the **1 Billion Digits** milestone! By leveraging the Chudnovsky algorithm and binary splitting, PiEngine efficiently handles massive numbers. Note that computing 1B digits and beyond is primarily dependent on available **RAM**, as the intermediate values in the binary splitting process grow significantly.

---

## Features

- Dual Implementations (Branch-separated):
  - **OCaml** (`main` branch): Utilizing the powerful Zarith library for clean, high-performance functional code.
  - **C** (`v3-C` branch): Leveraging GMP and MPFR for maximum low-level efficiency.
- Binary Splitting: Optimized series summation that reduces the complexity of large multiplications.
- High Performance: Designed to scale from thousands to billions of digits.
- Cross-Platform: Tested on Linux and macOS.
- Robust CLI: Complete control over precision, output, and verbose timing metrics.

---

## How the Algorithm Works

### Chudnovsky Series
PiEngine uses the world-record Chudnovsky algorithm, which converges at approximately 14 digits per term:

$$\frac{1}{\pi} = 12 \sum_{k=0}^{\infty} \frac{(-1)^k (6k)! (13591409 + 545140134k)}{(3k)!(k!)^3 640320^{3k+3/2}}$$

### Binary Splitting
To compute this series efficiently, we use Binary Splitting. Instead of computing each term and adding it to a running total, we recursively split the series into two halves. This allows us to use large, efficient multiplications (Karatsuba or FFT-based) provided by GMP and Zarith, significantly speeding up the process for millions of digits. For very large digit counts, the memory usage becomes the primary bottleneck.

---

## Roadmap and Next Goal: 10,000,000,000 Digits

Our next target is the 10 Billion Digits milestone. To reach this level of performance and handle the increasing computational load, future updates will focus on:

- Parallelization: Implementing multi-threaded binary splitting to utilize all available CPU cores.
- Memory Optimization: Reducing the memory footprint during the division and square root phases.
- Disk-based Computation: Exploring swapping strategies for digit counts that exceed available RAM.

---

## Getting Started

### Prerequisites

#### For OCaml (on `main` branch)
- OPAM
- Dune
- zarith library

```bash
opam install zarith dune
```

#### For C (on `v3-C` branch)
- GMP
- MPFR

```bash
# Ubuntu/Debian
sudo apt install libgmp-dev libmpfr-dev
# Arch Linux
sudo pacman -S gmp mpfr
```

---

## Build and Run

### OCaml Version (Default - `main` branch)
The OCaml version offers a balance of safety and performance.

```bash
dune build
dune exec bin/main.exe -- --digits 1000000 --verbose --output pi.txt
```

### C Version (Switch to `v3-C` branch)
The C version provides low-level control and direct access to GMP primitives.

```bash
git checkout v3-C
make
./piengine --digits 1000000 --verbose --output pi.txt
```

### CLI Options
Both versions support identical CLI flags:
- -d, --digits NUM: Number of digits to compute (default: 1000).
- -o, --output FILE: Write the result to a file instead of stdout.
- -v, --verbose: Display detailed timing and performance information.
- -h, --help: Show help message.

---

## Project Structure

The project is organized into two primary branches:

- **`main`**: Contains the OCaml implementation.
  - `bin/`: CLI application.
  - `lib/`: Core Chudnovsky logic.
  - `test/`: Regression and comparison tests.
- **`v3-C`**: Contains the C implementation.
  - `src/`: Core logic and CLI.
  - `tests/`: Unit tests.

---

## Milestones

| Milestone | Digits | Status |
| :--- | :--- | :--- |
| Milestone 1 | 1,000 | Complete |
| Milestone 2 | 1,000,000 | Complete |
| Milestone 3 | 100,000,000 | Complete |
| Milestone 4 | 1,000,000,000 | **Achieved** |
| **Milestone 5** | **10,000,000,000** | **In Progress / Planned** |

---

## Testing
We ensure correctness by comparing results between the OCaml and C implementations, as well as against known reference constants.

```bash
# Run OCaml tests (on main branch)
./test_pi.sh
```

---

*“Mathematics is the music of reason.”*
