#ifndef DSP_LECTURE_H
#define DSP_LECTURE_H

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <arm_acle.h>
#include <arm_neon.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h

#define N_MAX 65535
#define MAX_INT16 INT16_MAX
#define MIN_INT16 INT16_MIN

typedef struct { int16_t real,imag; } cint16_t;
typedef struct { int32_t real,imag; } cint32_t;

void fir_fast(int m, int n, int16_t* dst, int16_t* taps, int16_t* src);
void fir_ref(int m, int n, int16_t* dst, int16_t* taps, int16_t* src);

#endif /* DSP_LECTURE_H */
