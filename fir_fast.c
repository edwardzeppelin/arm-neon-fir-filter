#include "dsp-lecture.h"

void fir_fast(int m, int n, int16_t* dst, int16_t* taps, int16_t* src) {
    for (int i = 0; i < n; i++) {
        int64_t res = 0;
        int j = 0;
        
        // Определяем, сколько коэффициентов можно использовать
        int max_j = (i + 1 < m) ? i + 1 : m;
        
        for (; j <= max_j - 4; j += 4) {
            int16x4_t tap_vec = vld1_s16(&taps[j]);
            
            int16x4_t src_vec;
            src_vec = vset_lane_s16(src[i - j], src_vec, 0);
            src_vec = vset_lane_s16(src[i - j - 1], src_vec, 1);
            src_vec = vset_lane_s16(src[i - j - 2], src_vec, 2);
            src_vec = vset_lane_s16(src[i - j - 3], src_vec, 3);
            
            int32x4_t mul = vmull_s16(src_vec, tap_vec);
            
            int32x2_t sum = vadd_s32(vget_low_s32(mul), vget_high_s32(mul));
            sum = vpadd_s32(sum, sum);
            
            res += vget_lane_s32(sum, 0);
        }
        
        for (; j < max_j; j++) {
            res += (int64_t)src[i - j] * (int64_t)taps[j];
        }
        
        int32_t result = (int32_t)(res >> 15);
        
        if (result > MAX_INT16) dst[i] = MAX_INT16;
        else if (result < MIN_INT16) dst[i] = MIN_INT16;
        else dst[i] = (int16_t)result;
    }
}