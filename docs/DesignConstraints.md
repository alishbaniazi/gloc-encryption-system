# Design Constraints

This document outlines the constraints under which G-LOC was built. These restrictions were intentional — they force implementation of cryptographic primitives from scratch, building genuine understanding rather than relying on black-box libraries.

---

## Hard Constraints

### No Cryptographic Libraries
Standard cryptography libraries (`<openssl>`, `<botan>`, etc.) were strictly off-limits. Every mathematical operation — prime testing, modular arithmetic, coprimality — is implemented manually.

### No Standard Utility Libraries
Only `<iostream>` was permitted. This means:
- No `<cmath>` for math functions
- No `<algorithm>` for sorting or searching
- No `<string>` or string manipulation utilities
- No STL containers (`vector`, `map`, etc.)

### No String Type
All text handling is done through raw `char` arrays with manual indexing, null-termination, and buffer management.

### Integer Arithmetic Only
All cryptographic calculations use integer types. Floating point is not used anywhere in the pipeline.

---

## Implementation Decisions Driven by Constraints

### Prime Generation
Without any library support, primality is tested via trial division up to `n/2`. While not optimal for large numbers, it is correct and sufficient given the clamped prime range (11–61).

### Modular Exponentiation
The standard library provides no modular exponentiation. A custom iterative approach is used:
```
result = 1
repeat binding_exponent times:
    result = (result × value) % modulus
```
This avoids integer overflow by reducing at each step.

### Coprimality Check
Without GCD functions, coprimality is determined by finding all factors of both numbers and checking for any common factor. Arrays of fixed size 3600 are used (bounded by max system totient of 60×60).

### Fixed-Width Padding
The encrypted stream uses zero-padded numbers of fixed width (based on maximum possible session modulus digits). This ensures uniform parsing during decryption without any string-length utilities.

### 64-bit Seed Pattern
The PIN is stored as a repeating 64-bit pattern (`abcdabcdabcdabcd`) inside an `unsigned long long int`. Bit counting across even and odd positions is done via bitwise shift and mask operations.

---

## Known Limitations

- Prime search is linear and unoptimized — acceptable given the small clamped range
- Message size is bounded by a fixed buffer (`char[10000]`)
- Encrypted stream is transmitted as a single unbroken string (no whitespace support in decryption input)
- Security is academic — the small prime range (11–61) makes this unsuitable for real-world cryptographic use

---

## What This Demonstrates

Despite the constraints, the project successfully implements the full RSA conceptual pipeline:

1. Seed-based deterministic key generation
2. Public/private keypair derivation
3. Asymmetric encryption and decryption
4. Fixed-format data stream encoding

The constraints transform what could be a trivial library call into a ground-up systems programming exercise.
