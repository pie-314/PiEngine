# Migration Plan: PiEngine C to OCaml

This document outlines the strategic transition of the PiEngine from a C-based implementation to **OCaml**.

## 1. Rationale: Why OCaml?

While C provides low-level control, OCaml offers a unique balance of **performance** and **safety** that is ideal for complex mathematical engines.

- **Arbitrary Precision via Zarith:** OCaml's `Zarith` library is a thin, highly optimized wrapper around GMP, providing C-like performance for big integers with much safer memory management.
- **Type Safety:** Eliminate segment faults and buffer overflows that often plague high-precision C code.
- **Functional Clarity:** The recursive nature of the Binary Splitting algorithm is more naturally expressed in OCaml's functional paradigm.
- **Automatic Memory Management:** OCaml's Garbage Collector (GC) is highly tuned for the short-lived allocations typical of the Binary Splitting tree nodes.

---

## 2. Technical Architecture Mapping

| Feature | C Implementation | OCaml Implementation |
| :--- | :--- | :--- |
| **Big Integers** | `gmp.h` (`mpz_t`) | `Zarith` (`Z.t`) |
| **Floating Point** | `mpfr.h` (`mpfr_t`) | `Gmp` / `Mlgmpidl` or native floats |
| **Tree Logic** | Recursive C functions | Pure functional recursion |
| **CLI Parsing** | Manual `getopt` / `strcmp` | `Arg` module |
| **Build System** | `Makefile` | `Dune` |

---

## 3. Implementation Phases

### Phase 1: Environment Setup
- Install the OCaml toolchain via `opam`.
- Initialize a `dune` project.
- Install dependencies: `opam install zarith conf-gmp`.

### Phase 2: Core Mathematics (The Zarith Layer)
- Port the Binary Splitting base cases ($P, Q, T$).
- Implement the recursive `compute_bs` using OCaml's `Z` module.
- **Goal:** Match the mathematical outputs of the C implementation for $P, Q,$ and $T$.

### Phase 3: The Floating Point Finalizer
- Since OCaml's native `float` is 64-bit, we will use a dedicated MPFR wrapper (like `mlgmpidl`) for the final $\pi = \frac{C \sqrt{E} Q}{T}$ calculation to maintain billion-digit precision.

### Phase 4: CLI and I/O
- Implement the `-d` (digits) and `-o` (output) flags using the `Arg` module.
- Optimize string conversion for large outputs (using `Z.to_string`).

---

## 4. Performance Considerations

### The "Zarith" Advantage
OCaml's `Z.t` uses an "unboxed" representation for small integers and automatically switches to GMP for large ones. This reduces overhead significantly for the lower levels of the Binary Splitting tree where numbers are still small.

### Garbage Collection Tuning
For a 1 Billion digit run, we will need to tune the OCaml GC (`OCAMLRUNPARAM`) to ensure it doesn't trigger too frequently, as the intermediate nodes in the binary splitting tree can be quite large.

---

## 5. Success Criteria
1.  **Bit-for-Bit Parity:** The OCaml engine must produce the exact same digits as the verified C engine.
2.  **Memory Stability:** The OCaml version must stay within the 8GB RAM limit for 1 Billion digits.
3.  **Clean Build:** Zero warnings and a single `dune build` command.
