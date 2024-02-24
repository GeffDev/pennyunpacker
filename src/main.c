#include "utility.h"

#include "extract.h"

i32 main(i32 argc, u8 *argv[]) {
    printLog("pennyunpacker\n");

    for (i32 i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--rmheader") == 0) {
            remove_header = true;
        }
    }

    extractPacks();

    return 0;
}