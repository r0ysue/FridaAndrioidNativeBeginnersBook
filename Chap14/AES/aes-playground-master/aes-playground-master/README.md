# AES Playground

My experiments in understanding AES, Whitebox AES, and related attacks.

See `textbook_aes.py` and the contents of the `aes/` subdirectory for an AES128
implementation, which maps as closely to the FIPS 197 spec as possible.

### DISCLAIMER: This code is optimised for educational purposes. Do not use in a security context.

My goals are to:

- Implement textbook AES from first principles.

- Implement LUT-based AES.

- Try extracting keys from the LUTs.

- Replicate academic Whitebox AES implementations.

- Implement known attacks, from first principles.

- Invent/implement novel attacks! (can "Differential Fault Analysis Using Symbolic Execution" be applied to whitebox/LUT based implementations?)

# Resources

- AES: https://en.wikipedia.org/wiki/Advanced_Encryption_Standard

- FIPS 197 (NIST AES Standard): https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197.pdf

- Useful debugging tool: https://www.cryptool.org/en/cto-highlights/aes

- Understanding Galois Fields: https://en.wikipedia.org/wiki/Finite_field_arithmetic

- "A Tutorial on White-box AES": https://eprint.iacr.org/2013/104.pdf

# "To Research" list:

- Algebraic Fault Analysis

- Differential Fault Analysis Using Symbolic Execution

    - https://www.researchgate.net/publication/322089366_Differential_Fault_Analysis_Using_Symbolic_Execution

    - https://hardwear.io/document/flip-a-bit-grab-a-key-hardwear-io-2017-v1-1.pdf
