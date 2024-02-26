#include "extract.h"
#include "hash.h"
#include "utility.h"

FILE *data_packs[DATA_FILE_NUM] = {0};

bool remove_header = false;

i32 extractPacks() {
    u8 *file_name_base = "Pack/Data";

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

        data_packs[i - 1] = fopen(file_name, "rb+");
        if (data_packs[i - 1] == NULL) {
            printLog("ERROR: %s pack not found!", file_name);
            return 1;
        }
    }

    FILE *files = fopen("files.txt", "rb+");
    if (files == NULL) {
        printLog("ERROR: files.json not found!");
        return 1;
    }

    bool has_file_names = false;
    FILE *file_names = fopen("filenames.txt", "rb+");
    if (file_names == NULL) {
        printLog("filenames.txt not found, continuing with hashed file names.");
    } else {
        has_file_names = true;
    }

    u8 files_line_buf[0x80] = {0};
    u8 names_line_buf[0x80] = {0};

    i32 offset = 0;
    i32 pack = 0;
    u8 path[33] = {0};
    i32 size = 0;

    while (true) {
        // don't need id
        u8 *return_val = fgets(files_line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }

        // offset
        return_val = fgets(files_line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(files_line_buf, "offset %d", &offset) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        // pack
        return_val = fgets(files_line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(files_line_buf, "pack %d", &pack) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        // path
        return_val = fgets(files_line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(files_line_buf, "path %s", &path) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        bool file_name_found = false;
        u8 *file_name;
        if (has_file_names) {
            while (true) {
                return_val = fgets(names_line_buf, 0x80, file_names);
                if (return_val == NULL) {
                    printLog("no file name found for %s, continuing with hashed file name", path);
                    fseek(file_names, 0, SEEK_SET);
                    break;
                }

                names_line_buf[strlen(names_line_buf) - 2] = '\0';

                //i32 last_dir = 0;
                //i32 file_ext = 0;
                //bool found_ext = false;
                //for (i32 i = 0; i < strlen(names_line_buf); i++) {
                //    if (names_line_buf[i] == '/') {
                //        last_dir = i;
                //    }
                //    if (names_line_buf[i] == '.' && found_ext == false) {
                //        file_ext = i;
                //        found_ext = true;
                //    }
                //}

                //file_name = calloc(1, file_ext - (last_dir + 1) + 1);
                //strncpy(file_name, names_line_buf + last_dir + 1, file_ext - (last_dir + 1));

                //for (i32 i = 0; i < strlen(file_name); i++) {
                //    file_name[i] = tolower(file_name[i]);
                //}

                file_name = calloc(1, strlen(names_line_buf) + 1);
                strcpy(file_name, names_line_buf);

                i32 hash_buf[4] = {0};
                generateHash(hash_buf, file_name);

                u8 path_comp[33] = {0};
                sprintf(path_comp, "%08x%08x%08x%08x", hash_buf[0], hash_buf[1], hash_buf[2], hash_buf[3]);

                if (strcmp(path_comp, path) == 0) {
                    file_name_found = true;
                    fseek(file_names, 0, SEEK_SET);
                    break;
                } else {
                    free(file_name);
                }
            }
        }

        // size
        return_val = fgets(files_line_buf, 0x80, files);
        if (return_val == NULL) {
            printLog("reached end of files.txt");
            break;
        }
        if (sscanf(files_line_buf, "size %d", &size) != 1) {
            printLog("ERROR: failed to parse file");
            break;
        }

        u8 full_file_path[0x100];
        strcpy(full_file_path, "./");

        if (!file_name_found) {
            strcat(full_file_path, "Encrypted/");
            mkdir(full_file_path, 0777);

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

            mkdir(full_file_path, 0777);

            strcat(full_file_path, path);
        } else {
            strcat(full_file_path, "Data/");

            i32 dir_index = 0;
            for (i32 i = 0; i < strlen(file_name); i++) {
                if (file_name[i] == '/') {
                    dir_index = i;
                    strncat(full_file_path, file_name, dir_index);
                    mkdir(full_file_path, 0777);
                }
            }

            if (dir_index == 0) {
                strcat(full_file_path, file_name);
            } else {
                strcat(full_file_path, file_name + dir_index);
            }

            free(file_name);
        }

        FILE *unpacked_file = fopen(full_file_path, "wb+");

        if (remove_header) {
            offset += 0x30;
        } else {
            size += 0x30;
        }

        fseek(data_packs[pack - 1], offset, SEEK_SET);

        u8 *file_buffer;
        file_buffer = malloc(size);

        if (file_buffer == NULL) {
            printLog("ERROR: failed to allocate file buffer");
            break;
        }
        fread(file_buffer, size, 1, data_packs[pack - 1]);
        fwrite(file_buffer, size, 1, unpacked_file);
        free(file_buffer);

        offset = 0;
        pack = 0;
        memset(path, 0, 33);
        size = 0;

        fclose(unpacked_file);
    }

    for (i32 i = 1; i < DATA_FILE_NUM; i++) {
        fclose(data_packs[i - 1]);
    }

    return 0;
}

i32 packFiles() {
    // TODO

    return 0;
}