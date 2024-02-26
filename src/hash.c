#define CLOWNMD5_IMPLEMENTATION
#include "clownmd5/clownmd5.h"

#include "hash.h"

void generateHash(i32 *hash_buffer, u8 *string) {
    u32 len = strlen(string);

    ClownMD5_State state;
    ClownMD5_Init(&state);

    while (true) {
        if (len > 64) {
            ClownMD5_PushData(&state, string);
        } else {
            u8 final_block[64];
            memcpy(final_block, string, len);

            ClownMD5_PushFinalData(&state, final_block, len * 8, NULL);

            hash_buffer[0] = state.A;
            hash_buffer[1] = state.B;
            hash_buffer[2] = state.C;
            hash_buffer[3] = state.D;
            break;
        }

        string += 64;
        len -= 64;
    }
}