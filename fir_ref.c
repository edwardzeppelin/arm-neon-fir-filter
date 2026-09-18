#include "dsp-lecture.h"

void fir_ref(int m, int n, int16_t* dst, int16_t* taps, int16_t* src) {
	for (int i = 0; i < n; i++) {
		int32_t sum = 0;
		for (int j = 0; j < m; j++) {
			if (i - j >= 0) {
				sum += (int32_t)src[i - j] * (int32_t)taps[j];
			}
		}
		
		sum = sum >> 15;
		
		if (sum > MAX_INT16) {
			dst[i] = MAX_INT16;
		}
		else if (sum < MIN_INT16) {
			dst[i] = - MIN_INT16;
		}
		else {
			dst[i] = (int16_t)sum;
		}
	}
}
