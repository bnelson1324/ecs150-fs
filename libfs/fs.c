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
    if (block_disk_open(diskname) == -1) {
        perror("fs_mount: failure to open disk");
        return -1;
    }

    // load superblock
    superblock = malloc(BLOCK_SIZE);
    if (block_read(0, superblock) == -1) {
        perror("fs_mount: failure to read superblock");
        return -1;
    }

    // load fat
    fat = malloc(BLOCK_SIZE * superblock->fatBlkCount);
    for (size_t i = 0; i < superblock->fatBlkCount; i++) {
        if (block_read(i + 1, (uint8_t*)fat + i * BLOCK_SIZE) == -1) {
            perror("fs_mount: failure to read FAT");
            return -1;
        }
    }

    // load root
    rootFiles = malloc(BLOCK_SIZE);
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
    if (superblock == NULL) {
        perror("fs_mount: already unmounted");
        return -1;
    }

    free(superblock);
    superblock = NULL;
    free(fat);
    fat = NULL;
    free(rootFiles);
    rootFiles = NULL;

    if (block_disk_close() == -1) {
        perror("fs_umount: failure to close disk");
        return -1;
    }

    // TODO 3.2: return -1 if there are still open file descriptors

    return 0;
}

int fs_info(void) {
    if (superblock == NULL) {
        perror("fs_info: no disk open / no fs mounted");
        return -1;
    }

    printf("FS Info:\n");
    printf("total_blk_count=%d\n", superblock->totalBlkCount);
    printf("fat_blk_count=%d\n", superblock->fatBlkCount);
    printf("rdir_blk=%d\n", superblock->rootBlkIndex);
    printf("data_blk=%d\n", superblock->dataBlkStartIndex);
    printf("data_blk_count=%d\n", superblock->dataBlkCount);

    int fatEmpty = 0;
    for (int i = 0; i < superblock->dataBlkCount; i++) {
        if (fat[i] == 0)
            fatEmpty++;
    }
    printf("fat_free_ratio=%d/%d\n", fatEmpty, superblock->dataBlkCount);

    int rootDirEmpty = 0;
    for (int i = 0; i < FS_FILE_MAX_COUNT; i++) {
        if (rootFiles[i].fileName[0] == '\0')
            rootDirEmpty++;
    }
    printf("rdir_free_ratio=%d/%d\n", rootDirEmpty,FS_FILE_MAX_COUNT);

    return 0;
}
