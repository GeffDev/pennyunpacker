#ifndef EXTRACT_H
#define EXTRACT_H

#include "utility.h"

#define DATA_FILE_NUM 121

typedef struct {
    i32 id;
    i32 offset;
    u8 pack;
    u8 *path;
    i32 size;
} File;

// TODO: don't hardcode the number of data packs
extern FILE *data_packs[DATA_FILE_NUM];

i32 extractPacks();

#endif