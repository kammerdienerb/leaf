#include "internal.h"

u64 next_power_of_2(u64 x) {
    if (x == 0) {
        return 2;
    }

    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    x++;
    return x;
}

u64 measure_time_now_ms(void) {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return 1000ULL * tv.tv_sec + (tv.tv_usec / 1000ULL);
}

u64 measure_time_now_us(void) {
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return 1000000ULL * tv.tv_sec + (tv.tv_usec);
}
