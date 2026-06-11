# G-LOC: Guarded Link Over Combat

> *A custom asymmetric encryption system built from scratch in C++ — no libraries, no shortcuts.*

---

## Overview

G-LOC is a fully functional **RSA-style public-key cryptography engine** implemented in pure C++. Every component — from prime generation to modular exponentiation — is hand-crafted without any cryptographic libraries.

The system generates unique encryption/decryption keypairs derived from a user's 4-digit PIN, encrypts plaintext messages into a padded numeric data stream, and decrypts them back with full accuracy.

---

## The Challenge

This project was built under strict constraints that forced deep understanding of the underlying math:

- No cryptographic libraries
- No `<cmath>`, `<algorithm>`, or STL utilities
- Pure arithmetic, bitwise operations, and control flow only

Every algorithm you see here — primality testing, coprimality checking, modular exponentiation — was implemented from first principles.

---

## How It Works

### Key Generation Pipeline

```
4-digit PIN
    │
    ▼
Repeating 64-bit pattern (abcdabcdabcdabcd)
    │
    ▼
Bit counting → e (even positions) & o (odd positions)
    │
    ▼
e-th and o-th prime numbers → Handshake Primes (P1, P2)
    │
    ▼
Prime Refinement Algorithm → Refined P1, Refined P2
    │
    ▼
Session Modulus N = Refined_P1 × Refined_P2
System Totient  φ = (Refined_P1 - 1) × (Refined_P2 - 1)
    │
    ▼
Binding Exponent   → k-th number coprime to φ  [PUBLIC KEY]
Unbinding Exponent → modular inverse of binding exponent [PRIVATE KEY]
```

### Encryption (Vector Protocol)
Each character is transformed via:
```
Encrypted = (ASCII_value ^ Binding_Exponent) % Session_Modulus
```
Results are zero-padded to a fixed width and joined into a single `_`-separated data stream.

### Decryption (Angels Protocol)
The stream is split, parsed, and each token is reversed via:
```
Original_ASCII = (Encrypted_Number ^ Unbinding_Exponent) % Session_Modulus
```

---

## Features

- Unique keypair per PIN — same PIN always produces the same keys (deterministic)
- Zero-padded encrypted stream for uniform data transmission
- Full round-trip: encrypt → transmit → decrypt with 100% accuracy
- Custom primality testing, coprimality checking, and modular exponentiation
- Interactive menu: choose to encrypt or decrypt at runtime

---

## Getting Started

### Prerequisites
- Any C++ compiler (g++, MSVC, clang++)
- C++11 or later

### Build & Run
```bash
g++ -o gloc src/main.cpp
./gloc
```

### Sample Session
```
Enter your 4 digit PIN: 0774

Encryption Keypair {527, 77}
Decryption Keypair {527, 293}

------- MENU --------
0 = Exit
1 = Encryption
2 = Decryption
Enter your choice: 1

Enter the message to encrypt: ALPHA BRAVO

Encrypted data stream:
0322_0406_0352_0353_0342_0068_0388_0353_...
```

---

## Project Structure

```
gloc-encryption-system/
├── src/
│   └── main.cpp          # Full implementation
├── docs/
│   └── DesignConstraints.md
├── LICENSE
└── README.md
```

---

## Concepts Demonstrated

| Concept | Implementation |
|---|---|
| Asymmetric Cryptography | RSA-style public/private key pairs |
| Number Theory | Prime generation, coprimality, modular inverse |
| Bitwise Operations | 64-bit pattern analysis for seed generation |
| Modular Exponentiation | Custom iterative implementation |
| Data Encoding | Zero-padded fixed-width stream formatting |

---

## Constraints & Design Decisions

This system was designed under academic constraints that mirror real low-level systems programming challenges:

- All math implemented without standard library functions
- Fixed-size character arrays instead of `std::string`
- Manual input parsing and buffer management
- Deterministic key generation (same PIN → same keys, always)

---

License

MIT License — see LICENSE for details.
