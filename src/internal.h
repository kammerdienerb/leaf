#ifndef __INTERNAL_H__
#define __INTERNAL_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <locale.h>
#define __USE_XOPEN
#define _XOPEN_SOURCE
#include <wchar.h>


#include <GLFW/glfw3.h>

typedef uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;

#define likely(x)   (__builtin_expect(!!(x), 1))
#define unlikely(x) (__builtin_expect(!!(x), 0))

u64 next_power_of_2(u64 x);

u64 measure_time_now_ms(void);
u64 measure_time_now_us(void);

#define ERR(...) do {                                \
    fprintf(stderr, "[leaf] ERROR:   " __VA_ARGS__); \
    exit(1);                                         \
} while (0)

#define WARN(...) do {                                   \
    if (!options->quiet) {                               \
        fprintf(stderr, "[leaf] WARNING: " __VA_ARGS__); \
    }                                                    \
} while (0)

#define INFO(...) do {                                   \
    if (!options->quiet) {                               \
        printf(         "[leaf] INFO:    " __VA_ARGS__); \
    }                                                    \
} while (0)

#define TIME(...) do {                                          \
    if (!options->quiet) {                                      \
        u64 _TIME_ms;                                           \
        _TIME_ms = measure_time_now_ms() - start_ms;            \
        printf(         "[leaf] (%.3f): ", _TIME_ms / 1000.0f); \
        printf(         "" __VA_ARGS__);                        \
    }                                                           \
} while (0)

#define FPS_TO_MS(fps)   ((1000) * (1.0 / (float)(fps)))
#define FPS_CAP_MS       ((u64)(FPS_TO_MS(60)))

#define DISPLAY_DELAY_FRAMES   (1)

#endif
