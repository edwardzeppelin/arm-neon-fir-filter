#include "dsp-lecture.h"

void fir_fast(int m, int n, int16_t* dst, int16_t* taps, int16_t* src) {
	for (int i = 0; i < n; i++) {
		int32_t res = 0;
		int j;
		
		for (j = 0; j <= m - 8; j += 8) {
			int16x8_t src_data = vld1q_s16(&src[i - j]);
			int16x8_t tap_data = vld1q_s16(&taps[j]);
			int32x4_t low = vmull_s16(vget_low_s16(src_data), vget_low_s16(tap_data));
			int32x4_t high = vmull_s16(vget_high_s16(src_data), vget_high_s16(tap_data));
			res += vgetq_lane_s32(low, 0) + vgetq_lane_s32(low, 1) + vgetq_lane_s32(high, 0) + vgetq_lane_s32(high, 1);
		}
		
		for (; j < m; j++) {
			res += (int32_t)src[i - j] * (int32_t)taps[j];
		}
		
		if (res > MAX_INT16) {
			dst[i] = MAX_INT16;
		}
		else if (res < MIN_INT16) {
			dst[i] = MIN_INT16;
		}
		else {
			dst[i] = (int16_t)(res >> 15);
		}
	}
}