# Learning OCaml: Implementing the Chudnovsky Algorithm

This guide is designed to help you transition from C (GMP) to OCaml (Zarith). It covers the fundamental concepts, syntax differences, and the logic required to implement the Chudnovsky algorithm for $\pi$.

---

## 1. The Big Picture: Functional vs. Imperative
In C, you tell the computer **how** to do things (step-by-step memory updates). In OCaml, you describe **what** things are (transformations and expressions).

| Feature | C (GMP) | OCaml (Zarith) |
| :--- | :--- | :--- |
| **Memory** | Manual (`mpz_init`, `mpz_clear`) | Automatic (Garbage Collected) |
| **State** | Mutates variables (`mpz_add(a, a, b)`) | Creates new values (`let c = Z.add a b`) |
| **Returns** | via pointers (void functions) | via direct return values |

---

## 2. Zarith Syntax (The "BigInt" of OCaml)
Since we are dealing with millions of digits, standard `int` (63-bit) isn't enough. We use the `Z` module from the `zarith` library.

### Basic Arithmetic
There are two ways to write math in Zarith:
1.  **Function Style**: `Z.add x y`, `Z.mul x y`, `Z.pow x e`.
2.  **Operator Style (Recommended)**: Use a "Local Open" to use `+`, `-`, `*` directly with BigInts.

```ocaml
(* Standard math uses regular ints *)
let x = 5 + 10 

(* Zarith math using a Local Open *)
let y = Z.(of_int 5 + of_int 10)
```

### Important Functions
- `Z.of_int i`: Converts a standard OCaml `int` to a `Z.t`.
- `Z.of_string "123..."`: Converts a massive string to a `Z.t`.
- `Z.to_string z`: Converts a `Z.t` back to a string for printing.
- `Z.sqrt z`: Integer square root.

---

## 3. The Logic: Binary Splitting
The Chudnovsky algorithm uses a series. Instead of doing division at every step (which is slow), we use **Binary Splitting** to turn the series into a tree of multiplications.

### The Triplet (P, Q, T)
For any range of terms $[n_1, n_2)$, we want to compute three values:
- **P**: The product of the numerator terms.
- **Q**: The product of the denominator terms.
- **T**: The partial sum for this branch.

### The Recursive Step
To compute $(P, Q, T)$ for a large range $[a, b)$:
1. Find the midpoint $m = (a + b) / 2$.
2. Recursively find $(P_1, Q_1, T_1)$ for the left half $[a, m)$.
3. Recursively find $(P_2, Q_2, T_2)$ for the right half $[m, b)$.
4. Combine them:
   - $P = P_1 \times P_2$
   - $Q = Q_1 \times Q_2$
   - $T = (T_1 \times Q_2) + (P_1 \times T_2)$

---

## 4. OCaml Syntax Patterns for Implementation

### Pattern Matching & Tuples
OCaml can return multiple values at once using **Tuples**. You can "unpack" them easily:

```ocaml
(* Returning a triplet *)
let get_values () = (one, two, three)

(* Unpacking the triplet *)
let (p, q, t) = get_values ()
```

### Recursive Functions
Functions are not recursive by default in OCaml. You must use the `rec` keyword.

```ocaml
let rec factorial n =
  if n = 0 then 1 else n * factorial (n - 1)
```

### The "Shadowing" Trap
When you `open Z`, the `*` and `/` operators now expect BigInts. If you try to do `let x = 10 / 2`, it will fail because `10` and `2` are standard `int`.
**Solution**: Do your range/loop math (standard ints) outside the `Z` scope, or use `Z.of_int`.

---

## 5. Milestone Checklist for your Implementation
1.  **Constants**: Define $A, B, C^3/24$ as `Z.t` values.
2.  **Base Case**: In `compute_bs`, if $n_2 = n_1 + 1$, compute the values for a single term $k$.
3.  **Recursive Case**: Implement the midpoint split and the $P, Q, T$ combination logic.
4.  **Final Step**: In a `compute_pi` function:
    - Call `compute_bs 0 terms`.
    - Compute $\sqrt{10005}$ with enough precision (multiply by $10^{2 \times digits}$ before taking `Z.sqrt`).
    - Perform the final multiplication and division.
    - Convert to string and insert the decimal point.

## 6. Pro-Tip: Dune Configuration
To use Zarith, your `lib/dune` file must look like this:
```lisp
(library
 (name your_lib_name)
 (libraries zarith))
```
And your `dune-project` should list `zarith` in the `(depends ...)` section.
