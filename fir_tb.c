#include "dsp-lecture.h"

void random_vector(int n, cint16_t* v) {
    for (int i = 0; i < n; i++) {
        v[i] = (int16_t)(rand() % 256);
    }
}

void fir_tb(int seed, int num_runs) {
	
	int run, n, m;
	int16_t* dst = NULL;
	int16_t* taps = NULL;
	int16_t* src = NULL;
	
    srand48(seed);
    
    for (run = 0; run < num_runs; run++) {
        m = rand() % N_MAX;
		n = rand() % N_MAX;
        
        dst_ref = (int16_t*)realloc(dst_ref, n * sizeof(int16_t));
		dst_fast = (int16_t*)realloc(dst_fast, n * sizeof(int16_t));
        taps = (int16_t*)realloc(taps, m * sizeof(int16_t));
        src = (int16_t*)realloc(src, n * sizeof(int16_t));
		
        random_vector(m, taps);
        random_vector(n, src);

        clock_t ref_start = clock();
        fir_ref(m, n, dst_ref, taps, src);
        clock_t ref_time = clock() - ref_start;
        printf("Ref time: %ld ticks\n", (long)ref_time);

        clock_t fast_start = clock();
        fir_fast(m, n, dst_fast, taps, src);
        clock_t fast_time = clock() - fast_start;
        printf("Fast time: %ld ticks\n", (long)fast_time);
     
        bool error = false;
        for (int i = 0; i < n; i++) {
            if (dst_fast[i] != dst_ref[i]) {
                fprintf(stderr, "Error at index %d: ref=(%d) fast=(%d)\n", i, dst_ref[i], dst_fast[i]);
                error = true;
                break;
            }
        }
        
        if (!error) {
            printf("Run %d: OK (n=%d, m=%d)\n", run, n, m);
        }

        free(src);
        free(taps);
        free(dst_fast);
        free(dst_ref);
    }
}

int main() {
    fir_tb(16, 100);
    printf("Success\n");
    return 0;
}