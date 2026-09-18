#include "dsp-lecture.h"

void fir_fast(int m, int n, int16_t* dst, int16_t* taps, int16_t* src) {
    for (int i = 0; i < n; i++) {
        int64_t res = 0;  // Используем int64_t чтобы избежать переполнения
        
        // Определяем, сколько коэффициентов можно реально использовать для этого i
        int usable_m = (i + 1 < m) ? i + 1 : m;
        
        // Векторизованная часть (только если достаточно данных)
        int j = 0;
        for (; j <= usable_m - 8; j += 8) {
            // Загружаем 8 tap коэффициентов
            int16x8_t tap_data = vld1q_s16(&taps[j]);
            
            // Загружаем соответствующие 8 значений из src в правильном порядке
            int16_t src_temp[8];
            for (int k = 0; k < 8; k++) {
                src_temp[k] = src[i - j - k];
            }
            int16x8_t src_data = vld1q_s16(src_temp);
            
            // Умножаем и расширяем до 32 бит
            int32x4_t low = vmull_s16(vget_low_s16(src_data), vget_low_s16(tap_data));
            int32x4_t high = vmull_s16(vget_high_s16(src_data), vget_high_s16(tap_data));
            
            // Суммируем результаты из векторов
            int32x2_t low_sum = vadd_s32(vget_low_s32(low), vget_high_s32(low));
            int32x2_t high_sum = vadd_s32(vget_low_s32(high), vget_high_s32(high));
            int32x2_t total_sum = vadd_s32(low_sum, high_sum);
            
            res += vget_lane_s32(total_sum, 0) + vget_lane_s32(total_sum, 1);
        }
        
        // Скалярная обработка оставшихся коэффициентов
        for (; j < usable_m; j++) {
            res += (int64_t)src[i - j] * (int64_t)taps[j];
        }
        
        // Для оставшихся коэффициентов (если usable_m < m) - ничего не делаем,
        // так как для них нет соответствующих данных в src
        
        // Деление на 32768 (Q15 формат)
        int32_t result = (int32_t)(res >> 15);
        
        // Ограничение
        if (result > MAX_INT16) {
            dst[i] = MAX_INT16;
        }
        else if (result < MIN_INT16) {
            dst[i] = MIN_INT16;
        }
        else {
            dst[i] = (int16_t)result;
        }
    }
}