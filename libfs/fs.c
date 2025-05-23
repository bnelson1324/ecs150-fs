#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "disk.h"
#include "fs.h"

typedef struct __attribute__((__packed__)) {
    char signature[8];
    uint16_t totalBlkCount;
    uint16_t rootBlkIndex;
    uint16_t dataBlkStartIndex;
    uint16_t dataBlkCount;
    uint8_t fatBlkCount;
    char _padding[4079];
} superblock_t;

typedef struct __attribute__((__packed__)) {
    char fileName[FS_FILENAME_LEN];
    uint32_t fileSize;
    uint16_t firstDataBlkIndex;
    char _padding[10];
} rootFile_t;

/* state */
superblock_t *superblock;
uint16_t *fat;
rootFile_t *rootFiles;

int fs_mount(const char *diskname) {
    // allocate buffers
    superblock = malloc(BLOCK_SIZE);
    fat = malloc(BLOCK_SIZE * superblock->fatBlkCount);
    rootFiles = malloc(BLOCK_SIZE);

    // load metadata
    if (block_disk_open(diskname) == -1) {
        perror("fs_mount: failure to open disk");
        return -1;
    }
    if (block_read(0, superblock) == -1) {
        perror("fs_mount: failure to read superblock");
        return -1;
    }
    for (size_t i = 0; i < superblock->fatBlkCount; i++) {
        if (block_read(i + 1, &fat[i]) == -1) {
            perror("fs_mount: failure to read FAT");
            return -1;
        }
    }
    if (block_read(superblock->rootBlkIndex, rootFiles) == -1) {
        perror("fs_mount: failure to read root");
        return -1;
    }

    // err check
    if (memcmp(superblock->signature, "ECS150FS", 8) != 0) {
        perror("fs_mount: bad signature");
        return -1;
    }
    if (superblock->totalBlkCount != block_disk_count()) {
        perror("fs_mount: superblock block count mismatch");
        return -1;
    }
    return 0;
}

int fs_umount(void) {
    free(superblock);
    free(fat);
    free(rootFiles);

    if (block_disk_close() == -1) {
        perror("fs_umount: failure to close disk");
        return -1;
    }

    // TODO 3.2: return -1 if there are still open file descriptors

    return 0;
}

int fs_info(void) {
    /* TODO */

    /**
     * fs_info - Display information about file system
     *
     * Display some information about the currently mounted file system.
     *
     * Return: -1 if no underlying virtual disk was opened. 0 otherwise.
     */

    return 0;
}
