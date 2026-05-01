# PiEngine V3: Binary Splitting Implementation Plan

## 1. Overview
Version 3 of PiEngine transitions from an iterative $O(n^2)$ Chudnovsky implementation to a **Binary Splitting** approach. This optimization leverages the divide-and-conquer strategy to evaluate the series, which is significantly faster when combined with modern fast multiplication algorithms (like Karatsuba or FFT) available in the GMP library.

### Why Binary Splitting?
- **Iterative (V2):** Each new term is added to a high-precision sum. As the number of digits grows, we perform many multiplications/divisions at full precision.
- **Binary Splitting (V3):** We recursively split the series into two halves. This keeps the operands of multiplications roughly the same size, allowing the $O(n \log n)$ multiplication algorithms to shine. The complexity drops from $O(n^2)$ to $O(n \log^3 n)$.

---

## 2. Mathematical Foundation
The Chudnovsky formula for $1/\pi$ is:
$$\frac{1}{\pi} = 12 \sum_{k=0}^{\infty} \frac{(-1)^k (6k)! (A + Bk)}{(3k)! (k!)^3 (C)^{3k+3/2}}$$
Where:
- $A = 13591409$
- $B = 545140134$
- $C = 640320$

For binary splitting, we evaluate the partial sum $S(a, b) = \sum_{k=a}^{b-1} \frac{a_k}{b_k}$ by computing three integers $P(a, b)$, $Q(a, b)$, and $T(a, b)$ such that:
$$S(a, b) = \frac{T(a, b)}{Q(a, b)}$$

### Recursive Definitions
For a range $[a, b)$ with midpoint $m = \lfloor (a+b)/2 \rfloor$:
- $P(a, b) = P(a, m) \times P(m, b)$
- $Q(a, b) = Q(a, m) \times Q(m, b)$
- $T(a, b) = T(a, m) \times Q(m, b) + P(a, m) \times T(m, b)$

### Base Case ($b = a + 1$)
For $k = 0$:
- $P(0, 1) = 1$
- $Q(0, 1) = 1$
- $T(0, 1) = A$

For $k > 0$:
- $P(k, k+1) = -(6k-5)(2k-1)(6k-1)$
- $Q(k, k+1) = k^3 \cdot \frac{C^3}{24} = k^3 \cdot 10939058860032000$
- $T(k, k+1) = P(k, k+1) \cdot (A + Bk)$

---

## 3. Implementation Strategy

### Data Structures
We will use `mpz_t` (GMP Integers) for all intermediate $P, Q, T$ values. These numbers will grow extremely large, but GMP handles them efficiently.

### Function Signature
```c
void compute_bs(long a, long b, mpz_t P, mpz_t Q, mpz_t T);
```

### The Algorithm
1.  **Base Case:** If $b = a + 1$:
    - Compute $P, Q, T$ directly using the base case formulas.
2.  **Recursive Step:**
    - Let $m = (a + b) / 2$.
    - Recursively call `compute_bs(a, m, P1, Q1, T1)`.
    - Recursively call `compute_bs(m, b, P2, Q2, T2)`.
    - $T = T_1 Q_2 + P_1 T_2$.
    - $P = P_1 P_2$.
    - $Q = Q_1 Q_2$.
    - Clear temporary variables $P_1, Q_1, T_1, P_2, Q_2, T_2$.

### Tips for C implementation (GMP/MPFR)
- Use `mpz_t` for $P, Q, T$.
- Use `mpz_mul` for multiplications.
- Use `mpz_addmul` or `mpz_add(T, T1*Q2, P1*T2)` for $T$.
- Base case $k > 0$:
  - $P(k, k+1) = -(6k-5)(2k-1)(6k-1)$
  - $Q(k, k+1) = k^3 \cdot 10939058860032000$
  - $T(k, k+1) = P(k, k+1) \cdot (A + Bk)$
- To compute $P(k, k+1)$ efficiently:
  ```c
  mpz_set_si(P, 6*k - 5);
  mpz_mul_si(P, P, 2*k - 1);
  mpz_mul_si(P, P, 6*k - 1);
  mpz_neg(P, P);
  ```

### Final Computation
Once $T(0, N)$ and $Q(0, N)$ are computed:
$$\pi = \frac{426880 \sqrt{10005} \cdot Q(0, N)}{T(0, N)}$$
- Perform $\sqrt{10005}$ at the required precision using `mpfr_sqrt_ui`.
- Convert $T$ and $Q$ to `mpfr` using `mpfr_set_z`.
- Perform the final multiplication and division.

---

## 4. Performance Optimizations
- **Memory Management:** In the recursive function, ensure that $P, Q, T$ are cleared or reused to avoid memory leaks.
- **Base Case Tuning:** For very small $b-a$, the overhead of recursion might be higher than a simple iterative loop. However, for Chudnovsky, even $b-a=1$ is usually fine as the number of terms is relatively small (14 digits per term).
- **Parallelism:** The recursive structure is naturally suited for multi-threading. We can compute the two halves of the tree in parallel.
- **Pre-calculating Constants:** $C^3/24$ and other constants should be initialized once.

---

## 5. Directory & File Changes
- `src/chudnovsky.c`: Replace the iterative `compute_pi` with the binary splitting version.
- `src/chudnovsky.h`: Update function declarations.
- `Makefile`: Ensure GMP and MPFR are linked.

---

## 6. Verification Plan
1.  **Unit Test:** Compare the output of V3 with V2 for small digit counts (e.g., 100, 1000, 10000).
2.  **Precision Check:** Ensure `set_precision` accounts for the necessary guard bits.
3.  **Benchmarking:** Measure time for 10^5 and 10^6 digits to confirm the $O(n \log^3 n)$ scaling.

---

## 7. Helpful Constants & Ratios (Quick Ref)
- $C = 640320$
- $C^3/24 = 10939058860032000$
- $A = 13591409$
- $B = 545140134$
- Digits per term $\approx 14.18$. For $D$ digits, use $N = \lfloor D/14.18 \rfloor + 1$ terms.
