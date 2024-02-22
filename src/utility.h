#ifndef UTILITY_H
#define UTILITY_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// better types lol
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define CLAMP(x, min, max) (x > max ? max : (x < min ? min : x))

// fixed point stuff
#define TO_FIXED(x) ((x) << 16)
#define FROM_FIXED(x) ((x) >> 16)

#ifdef __linux__
#include <sys/stat.h>
#define mkdir(dir, mode) mkdir(dir, mode)
#elif defined _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir, mode)
#endif

static inline void printLog(const char *message, ...) {
    char buffer[512];
    va_list args;

    va_start(args, message);
    vsprintf(buffer, message, args);
    printf("%s\n", buffer);

    va_end(args);
}

#endif