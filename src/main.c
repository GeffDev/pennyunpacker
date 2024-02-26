#include "utility.h"

#include "extract.h"

i32 main(i32 argc, u8 *argv[]) {
    printLog("pennyunpacker\n");

    for (i32 i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--extractheader") == 0) {
            remove_header = false;

            if (extractPacks() != 0) {
                printLog("ERROR: failed to extract files!");
                return 1;
            }
        } else if (strcmp(argv[i], "--extract") == 0) {
            remove_header = true;

            if (extractPacks() != 0) {
                printLog("ERROR: failed to extract files!");
                return 1;
            }
        } else if (strcmp(argv[i], "--pack") == 0) {
            if (packFiles() != 0) {
                printLog("ERROR: failed to pack files!");
                return 1;
            }
            return 0;
        }
    }

    return 0;
}