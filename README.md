# ARM NEON Q15 FIR Filter Optimization

Performance benchmark and verification suite for a 16-bit Q15 **FIR (Finite Impulse Response) Filter** accelerated using **ARM NEON SIMD intrinsics**.

## Key Features
* **Q15 Fixed-Point FIR Filtering**: Computes output samples $y[i] = \sum_{j=0}^{m-1} x[i-j] \cdot h[j]$ with Q15 scaling (`>> 15`) and range clamping (`MAX_INT16` / `MIN_INT16`).
* **ARM NEON SIMD Acceleration**:
  * Dual vectorization strategies using 64-bit (`int16x4_t`) and 128-bit (`int16x8_t`) NEON registers.
  * Parallel coefficient multiplication using `vmull_s16`.
  * Pairwise vector addition using `vpadd_s32` and `vadd_s32` for fast reduction.
  * High-precision accumulation in 64-bit integers (`int64_t`) to prevent intermediate arithmetic overflow.
* **Scalar Tail & Edge Handling**: Gracefully handles boundary conditions when buffer samples $i < m$ or remaining taps are not multiples of SIMD vector size.
* **Automated Verification**: Benchmark testbench measures performance against a reference scalar C implementation and verifies numeric accuracy across randomized datasets.

## Project Structure
* `fir_fast.c` — Optimized FIR filter using 64-bit / 128-bit ARM NEON vectorization.
* `fir_ref.c` — Scalar baseline C implementation of the FIR filter.
* `fir_tb.c` — Automated testbench with performance timer (`clock()`) and verification logic.
* `dsp-lecture.h` — Data structures and function prototypes.

## Build & Run

```bash
gcc -O2 -mfpu=neon fir_tb.c fir_fast.c fir_ref.c -o neon_fir_test
./neon_fir_test
```
