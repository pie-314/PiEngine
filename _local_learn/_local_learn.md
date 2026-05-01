# Local Learning & Transition Notes

## OCaml Transition Strategy (V3)

### 1. Versioning & Branching
- **Branching:** **YES**. Create a new branch named `ocaml-migration` or `v3-ocaml`. Do not overwrite the `main` branch yet. This allows you to compare the C and OCaml outputs side-by-side.
- **Release:** Finalize the C version as **V2.1** (Stable). Mark the OCaml transition as the development phase for **V3.0**.

### 2. Step-by-Step Implementation (V3)

#### Step 1: Environment Setup
- Install the OCaml toolchain: `sudo apt install opam` (on Debian).
- Initialize opam: `opam init && eval $(opam env)`.
- Install dependencies: `opam install dune zarith mlgmpidl`.
- Create a `dune-project` file.

#### Step 2: The "Minimum Viable Pi" (MVP)
- Implement a simple recursive `compute_bs` using the `Z` module from `Zarith`.
- Don't worry about CLI flags or multi-threading yet. Just hardcode a "100 digits" run.
- **Verification:** Ensure the `P, Q, T` integers for a small range (e.g., 0 to 10) are identical to the C version.

#### Step 3: Architecture Mapping
- **C Structs -> OCaml Records/Tuples:** The `P, Q, T` triplet should be a simple tuple `(Z.t * Z.t * Z.t)`.
- **C Loops/Recursion -> OCaml Recursion:** Use `let rec` for the binary splitting tree.

#### Step 4: The Final 64-bit precision
- Integrate `mlgmpidl` (OCaml MPFR wrapper) to handle the final square root and division.
- This is where you will set the `prec` to $3.32 \times \text{digits}$.

#### Step 5: I/O Optimization
- OCaml's `Printf` can be slow for billions of digits. Plan to use `Buffer.t` or direct channel writes for the final output string.

### 3. Key Learnings (C to OCaml)
- **Memory:** OCaml's GC is great, but for 1B digits, we must avoid "leaking" large Zarith objects in the recursion.
- **Types:** OCaml will catch the "Int vs Long" precision issues at compile time via its type system.

### 4. Progress Log
- [ ] Phase 1: Opam & Dune setup.
- [ ] Phase 2: Zarith base case implementation.
- [ ] Phase 3: Recursive merge logic.
- [ ] Phase 4: MPFR finalizer.
