# The Mathematics of Binary Splitting

Before writing code, we need to understand the "Math Magic" that makes Binary Splitting work. It all starts with 4th-grade fractions.

---

## 1. The Core Idea: Adding Fractions
How do you add two fractions?
$$\frac{1}{2} + \frac{1}{3} = \frac{(1 \times 3) + (1 \times 2)}{2 \times 3} = \frac{5}{6}$$

In general, adding any two fractions looks like this:
$$\frac{T_1}{Q_1} + \frac{T_2}{Q_2} = \frac{T_1 Q_2 + T_2 Q_1}{Q_1 Q_2}$$

Binary Splitting is just doing this for **thousands of fractions at once**, but in a very organized way.

---

## 2. A Simple Example: Calculating $e$
The formula for $e$ is a sum of terms:
$$e = \frac{1}{0!} + \frac{1}{1!} + \frac{1}{2!} + \frac{1}{3!} + \dots$$

Let's look at just the first 4 terms:
$$S = \frac{1}{1} + \frac{1}{1} + \frac{1}{2} + \frac{1}{6}$$

### Step A: Group them in pairs
- Pair 1: $(\frac{1}{1} + \frac{1}{1}) = \frac{1+1}{1} = \frac{2}{1}$
- Pair 2: $(\frac{1}{2} + \frac{1}{6}) = \frac{6+2}{12} = \frac{8}{12}$

### Step B: Add the results
$$\frac{2}{1} + \frac{8}{12} = \frac{(2 \times 12) + (8 \times 1)}{1 \times 12} = \frac{24+8}{12} = \frac{32}{12} = 2.666...$$

**Observation:** In every step, we are just calculating a new Numerator ($T$) and a new Denominator ($Q$).

---

## 3. The Chudnovsky Formula (Mathematical View)
The Chudnovsky formula looks scary:
$$\sum_{k=0}^{\infty} \frac{(-1)^k (6k)! (A + Bk)}{(3k)! (k!)^3 C^{3k}}$$

But we can rewrite it as a simple sum of terms:
$$S = \sum_{k=0}^{\infty} \frac{a_k}{b_k}$$

Where $a_k$ is the top part and $b_k$ is the bottom part.

### The Binary Splitting "Trick"
We define three mathematical functions for a range of terms from $a$ to $b$:
1. **$P(a, b)$**: The product of the "connecting" factors in the numerators.
2. **$Q(a, b)$**: The product of the denominators.
3. **$T(a, b)$**: The combined numerator for the whole range.

### The Recursive Laws
If you split a range $[a, b)$ into a Left half $[a, m)$ and a Right half $[m, b)$:
1. $P_{total} = P_{left} \times P_{right}$
2. $Q_{total} = Q_{left} \times Q_{right}$
3. $T_{total} = (T_{left} \times Q_{right}) + (P_{left} \times T_{right})$

**Wait, why is $T$ different?**
Look back at our fraction example: $\frac{T_1}{Q_1} + \frac{P_1 \cdot T_2}{Q_1 \cdot Q_2}$.
Because the terms in the Chudnovsky series are "chained" together (the denominator of the 2nd term depends on the 1st), we need $P$ to "link" them correctly.

---

## 4. Step-by-Step Breakdown for Chudnovsky

### The Base Case (k)
For a single term $k$, the math tells us:
- $P_k = -(6k-5)(2k-1)(6k-1)$  *(These are the factors that change every step)*
- $Q_k = k^3 \times (\text{Constant})$
- $T_k = P_k \times (A + Bk)$

### The Tree Structure
Imagine calculating 8 terms:
- Level 1: Calculate 8 individual $(P, Q, T)$ values.
- Level 2: Combine them into 4 pairs.
- Level 3: Combine those into 2 pairs.
- Level 4: Combine into 1 final result.

By the time you reach the top of the tree, $T$ and $Q$ are massive integers.
Final step: $\pi = \frac{\text{Constant} \times Q}{T}$

---

## 5. Summary: Math to Programming
- **Integers only:** Notice that $P, Q,$ and $T$ are all integers. We don't use decimals until the very last division!
- **Balanced Work:** By splitting the range in half, we make sure the "Left" math and "Right" math are about the same size. This is the secret to the speed.
