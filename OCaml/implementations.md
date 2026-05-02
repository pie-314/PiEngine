# Chudnovsky Algorithm: C vs OCaml Implementation

This document explains the Chudnovsky algorithm for calculating $\pi$ and provides a comparison between the C (GMP/MPFR) and OCaml (Zarith) implementations.

## 1. The Constants
Both versions start with the same mathematical constants. In OCaml, these must be lowercase.

| Constant | Value | Description |
| :--- | :--- | :--- |
| `a` | 13591409 | Part of the linear term |
| `b` | 545140134 | Part of the linear term |
| `c3` | $640320^3$ | Used in the denominator |
| `d` | 426880 | Final multiplication factor |
| `e` | 10005 | The square root term ($\sqrt{10005}$) |

### Parallel Snippets
**C (Pre-processor):**
```c
#define A 13591409
#define C3 10939058860032000
```

**OCaml (Module values):**
```ocaml
let a = Z.of_int 13591409
let c3 = Z.of_string "10939058860032000"
```

---

## 2. Binary Splitting (`compute_bs`)
This is the heart of the algorithm. It splits the series into a tree, multiplying terms together to avoid expensive divisions until the very end.

### Logic Flow
We compute three large integers at each step:
- **P**: The numerator product.
- **Q**: The denominator product.
- **T**: The partial sum term.

### C vs OCaml Parallels

**C (Manual Memory Management):**
In C, you must initialize and clear every variable.
```c
mpz_t temp1, temp2;
mpz_inits(temp1, temp2, NULL);
mpz_mul(temp1, T1, Q2);
mpz_mul(temp2, P1, T2);
mpz_add(T, temp1, temp2);
mpz_clears(temp1, temp2, NULL);
```

**OCaml (Functional & Garbage Collected):**
In OCaml, you write expressions. The Garbage Collector (GC) handles memory.
```ocaml
let t = Z.((t1 * q2) + (p1 * t2))
```

---

## 3. Why Zarith?
Standard OCaml `int` is 63-bit. $\pi$ to 1 million digits involves numbers with millions of bits. `Zarith` (the `Z` module) is an OCaml wrapper around the same **GMP** library used in your C code.

### Key Syntax Parallels
| Operation | C (GMP) | OCaml (Zarith) |
| :--- | :--- | :--- |
| Addition | `mpz_add(r, a, b)` | `let r = Z.add a b` or `Z.(a + b)` |
| Multiplication | `mpz_mul(r, a, b)` | `let r = Z.mul a b` or `Z.(a * b)` |
| Power | `mpz_pow_ui(r, a, e)` | `let r = Z.pow a e` |
| Initialization | `mpz_init(x)` | Not needed (just `let`) |

---

## 4. Implementation Steps for OCaml

1. **Install Zarith**: `opam install zarith`
2. **Update Dune**: Add `zarith` to the `libraries` list in `lib/dune`.
3. **Recursive `compute_bs`**:
   ```ocaml
   let rec compute_bs a b =
     if Z.(equal (add a one) b) then
       (* Base case: compute P, Q, T for a single k *)
       (p, q, t)
     else
       (* Recursive case: split and combine *)
       let m = Z.((a + b) / (of_int 2)) in
       let (p1, q1, t1) = compute_bs a m in
       let (p2, q2, t2) = compute_bs m b in
       let p = Z.(p1 * p2) in
       let q = Z.(q1 * q2) in
       let t = Z.((t1 * q2) + (p1 * t2)) in
       (p, q, t)
   ```

## 5. Summary of Differences
1. **Safety**: OCaml's type system prevents many "off-by-one" or null pointer errors common in C.
2. **Conciseness**: What takes 50 lines in C often takes 10 in OCaml due to operator overloading (`Z.(...)`).
3. **Performance**: Both use GMP under the hood, so for large calculations, the speed is nearly identical.
