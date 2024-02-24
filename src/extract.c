#include "extract.h"
#include "utility.h"

FILE *data_packs[DATA_FILE_NUM] = {0};

bool remove_header = true;

i32 extractPacks() {
    const u8 *file_name_base = "Pack/Data";

    for (i32 i = 1; i <= DATA_FILE_NUM; i++) {
        u8 file_name[64];
        strcpy(file_name, file_name_base);

        if (i <= 9) {
            u8 u8_buf[2] = {0};
            sprintf(u8_buf, "%d", i);

            strcat(file_name, "00");
            strcat(file_name, u8_buf);
            strcat(file_name, ".rsdk");
        } else if (i <= 99) {
            u8 u8_buf[3] = {0};
            sprintf(u8_buf, "%d", i);

            strcat(file_name, "0");
            strcat(file_name, u8_buf);
            strcat(file_name, ".rsdk");
        } else if (i <= 999) {
            u8 u8_buf[4] = {0};
            sprintf(u8_buf, "%d", i);

            strcat(file_name, u8_buf);
            strcat(file_name, ".rsdk");
        }

        data_packs[i] = fopen(file_name, "rb+");
        if (data_packs[i] == NULL) {
            printLog("ERROR: %s pack not found!", file_name);
            return 1;
        }
    }

    FILE *files = fopen("files.txt", "rb+");
    if (files == NULL) {
        printLog("ERROR! files.json not found!");
        return 1;
    }

    u8 line_buf[0x80] = {0};

    i32 offset = 0;
    i32 pack = 0;
    // len of hash
    u8 path[33] = {0};
    i32 size = 0;

    while (true) {
        // don't need id
        u8 *return_val = fgets(line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }

        // offset
        return_val = fgets(line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(line_buf, "offset %d", &offset) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        // pack
        return_val = fgets(line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(line_buf, "pack %d", &pack) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        // path
        return_val = fgets(line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(line_buf, "path %s", &path) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        // size
        return_val = fgets(line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(line_buf, "size %d", &size) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        u8 full_file_path[0x100];
        strcpy(full_file_path, "./");
        strcat(full_file_path, file_name_base);

        if (pack <= 9) {
            u8 u8_buf[2] = {0};
            sprintf(u8_buf, "%d", pack);

            strcat(full_file_path, "00");
            strcat(full_file_path, u8_buf);
            strcat(full_file_path, "/");
        } else if (pack <= 99) {
            u8 u8_buf[3] = {0};
            sprintf(u8_buf, "%d", pack);

            strcat(full_file_path, "0");
            strcat(full_file_path, u8_buf);
            strcat(full_file_path, "/");
        } else if (pack <= 999) {
            u8 u8_buf[4] = {0};
            sprintf(u8_buf, "%d", pack);

            strcat(full_file_path, u8_buf);
            strcat(full_file_path, "/");
        }

        // meh
        mkdir(full_file_path, 0777);

        strcat(full_file_path, path);
        FILE *unpacked_file = fopen(full_file_path, "wb+");

        if (remove_header) {
            offset += 0x30;
        }

        fseek(data_packs[pack], offset, SEEK_SET);

        u8 *file_buffer;
        if (remove_header) {
            file_buffer = malloc(size);
        } else {
            // size doesn't account for file header
            file_buffer = malloc(size + 0x30);
        }

        if (file_buffer == NULL) {
            printLog("ERROR: failed to allocate file buffer");
            break;
        }
        fread(file_buffer, size, 1, data_packs[pack]);
        fwrite(file_buffer, size, 1, unpacked_file);
        free(file_buffer);

        offset = 0;
        pack = 0;
        memset(path, 0, 33);
        size = 0;

        fclose(unpacked_file);
    }

    for (i32 i = 1; i < DATA_FILE_NUM; i++) {
        fclose(data_packs[i]);
    }

    return 0;
}