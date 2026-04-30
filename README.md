# PiEngine

## High Precision π Calculator in C

PiEngine is a high-precision π (pi) calculator written in C using the **Chudnovsky algorithm** with the **GMP** and **MPFR** libraries for arbitrary-precision arithmetic.

This project is built as a systems + mathematics project to learn:

* Numerical algorithms
* High precision arithmetic
* Performance optimization
* Large number computation
* Clean C project structure

---

## Project Goals

PiEngine is being developed in multiple versions:

| Version | Target              | Method                        |
| ------- | ------------------- | ----------------------------- |
| V1      | 100,000 digits      | Basic Chudnovsky (factorials) |
| V2      | 1,000,000 digits    | Recurrence optimization       |
| V3      | 10,000,000 digits   | Binary splitting              |
| V4      | 100,000,000+ digits | FFT + Multi-threading         |

### Current Version: V2

**Goal:** Compute **1,000,000 digits of π** correctly and reliably.

PiEngine has now reached the **1,000,000 digits** milestone.
This version focuses on improving performance while preserving correctness and clean project structure.

---

## How The Algorithm Works

PiEngine uses the **Chudnovsky series**:

![Chudnovsky formula](https://latex.codecogs.com/png.image?%5Cdpi%7B160%7D%20%5Cpi%20%3D%20%5Cfrac%7B426880%5Csqrt%7B10005%7D%7D%7B%5Csum_%7Bk%3D0%7D%5E%7B%5Cinfty%7D%20%5Cmathrm%7Bterm%7D_k%7D)

The individual term is:

![Chudnovsky term formula](https://latex.codecogs.com/png.image?%5Cdpi%7B160%7D%20%5Cmathrm%7Bterm%7D_k%20%3D%20%5Cfrac%7B(-1)%5Ek(6k)%21(13591409%20%2B%20545140134k)%7D%7B(3k)%21(k!)%5E3%20640320%5E%7B3k%7D%7D)

Each term of the series adds approximately **14 digits** of precision.

The program uses:

* **GMP (mpz_t)** → big integers (factorials, powers)
* **MPFR (mpfr_t)** → high precision floating point (division, square root, final π)

Flow of computation:

```
Compute C = 426880 * sqrt(10005)

sum = 0
for k = 0 to N:
    compute term_k using factorial formula
    sum += term_k

pi = C / sum
```

---

## Project Structure

```
PiEngine/
├── Makefile
├── README.md
├── src/
│   ├── main.c          → Program entry point
│   ├── chudnovsky.c    → Pi calculation logic
│   ├── chudnovsky.h
│   ├── cli.c           → Command line interface
│   ├── cli.h
│   ├── utils.c         → Timer, progress, helpers
│   ├── bigint.c        → Custom BigInt (future use)
│   ├── bigint.h
│
└── tests/
    └── test_pi.c
```

---

## Build Instructions

### Install Dependencies

**Arch Linux**

```bash
sudo pacman -S gmp mpfr
```

**Ubuntu / Debian**

```bash
sudo apt install libgmp-dev libmpfr-dev
```

---

### Build

```bash
make
```

### Run

```bash
./piengine
```

---

## Current Limitations (V2)

Version 2 improves performance over the basic factorial-based approach, but it is still not the final architecture for very large digit counts.

Because of this, Version 2 is expected to work efficiently up to:

> **~1,000,000 digits**

Future versions will implement faster algorithms.

---

## Future Improvements

Planned upgrades:

* [ ] CLI arguments (`--digits`, `--output`)
* [ ] Save π to file
* [ ] Timing / benchmarking
* [ ] Progress indicator
* [ ] Recurrence formula (faster term calculation)
* [ ] Binary splitting algorithm
* [ ] Multi-threaded computation

---

## Milestones

| Milestone   | Target           |
| ----------- | ---------------- |
| Milestone 1 | 1,000 digits     |
| Milestone 2 | 10,000 digits    |
| Milestone 3 | 100,000 digits   |
| Milestone 4 | 1,000,000 digits |

---

## Summary

PiEngine is not just a π calculator.
It is a project to explore **high-precision computation, numerical methods, and performance optimization in C**.

**Current Milestone:** 1,000,000 digits achieved
**Next Goal:** 10,000,000 digits
