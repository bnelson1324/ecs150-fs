#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "disk.h"
#include "fs.h"

typedef struct __attribute__((__packed__)) {
    char signature[8];
    uint16_t blockCount;
    uint16_t rootBlkIndex;
    uint16_t dataBlkStartIndex;
    uint16_t dataBlkCount;
    uint8_t fatBlkCount;
    char padding[4079];
} superblock_t;

typedef struct __attribute__((__packed__)) {
    // TODO
} fat_t;

typedef struct __attribute__((__packed__)) {
    // TODO
} rootDir_t;

/* state */
superblock_t *superblock;


int fs_mount(const char *diskname) {
    // allocate buffers
    superblock = malloc(BLOCK_SIZE);

    // load metadata
    if (block_disk_open(diskname) == -1) {
        perror("fs_mount: failure to open disk");
        return -1;
    }
    if (block_read(BLOCK_SIZE, superblock) == -1) {
        perror("fs_mount: failure to read superblock");
        return -1;
    }
    // TODO: load fat, root

    // err check
    if (memcmp(superblock->signature, "ECS150FS", 8) != 0) {
        perror("fs_mount: bad signature");
        return -1;
    }
    if (superblock->blockCount != block_disk_count()) {
        perror("fs_mount: superblock block count mismatch");
        return -1;
    }
    return 0;
}

int fs_umount(void) {
    /**
     * fs_umount - Unmount file system
     *
     * Unmount the currently mounted file system and close the underlying virtual
     * disk file.
     *
     * Return: -1 if no FS is currently mounted, or if the virtual disk cannot be
     * closed, or if there are still open file descriptors. 0 otherwise.
     */

    /* TODO */
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
