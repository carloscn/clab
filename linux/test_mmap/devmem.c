#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdbool.h>

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

#define utils_printf printf("[devmem_tool] "); printf
#define utils_fprintf fprintf(stderr, "[devmem_tool] "); fprintf

#define MAP_SIZE (4096UL)              // Page size
#define MAP_MASK (MAP_SIZE - 1)       // MAP_MASK = 0XFFF
#define BUFFER_SIZE (MAP_SIZE + MAP_SIZE)

typedef enum operation_t {
    OP_READ = 0,
    OP_WRITE
} IO_TYPE_T;

static inline void devmem_usage(void)
{
    utils_printf("Usage ./devmem_tool read/write 0xfa0000 20\n");
    utils_printf("The read/write is the command type, read or write data to memory\n");
    utils_printf("The 0xfa2000 is the memory physical address\n");
    utils_printf("The 20 is read/write data length, uint is 4 byte, so it is total 20 * 4 = 80 bytes\n");
    utils_printf("Usage:  ./devmem_tool read 0xfa0000 20\n");
    utils_printf("Usage:  ./devmem_tool write 0xfa0000 20\n");
}

static int32_t devmem_io(intptr_t physical_addr, uint64_t *buffer, size_t *io_len, IO_TYPE_T op)
{
    int32_t ret = 0;
    int32_t fd = 0;
    size_t i = 0;
    size_t offset = 0;
    size_t len = 0;
    intptr_t addr = physical_addr;
    void *map_base = NULL;
    void *virt_addr = NULL;

    if (NULL == buffer ||
        NULL == io_len ||
        0 == *io_len ||
        op >= 2) {
        ret = -1;
        utils_printf("%s %s %d, input error\n",
                __FILE__, __func__, __LINE__);
        goto finish;
    }

    len = *io_len;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        ret = -1;
        utils_printf("open /dev/mem failed\n");
        utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",
                        __LINE__, __FILE__, errno, strerror(errno));
        goto finish;
    }

    if (op == OP_READ) {
        map_base = mmap(0,
                        MAP_SIZE, PROT_READ,
                        MAP_SHARED,
                        fd, addr & ~MAP_MASK);
    } else {
        map_base = mmap(0,
                        MAP_SIZE, PROT_WRITE,
                        MAP_SHARED,
                        fd, addr & ~MAP_MASK);
    }
    if ((void *) -1 == map_base) {
        ret = -1;
        utils_printf("mmap /dev/mem failed\n");
        utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",
                        __LINE__, __FILE__, errno, strerror(errno));
        goto fd_finish;
    }

    for (i = 0; i < len; i ++) {
        // turn the page
        if (offset >= MAP_MASK) {
            offset = 0;
            ret = munmap(map_base, MAP_SIZE);
            if (-1 == ret) {
                utils_printf("munmap /dev/mem failed\n");
                utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",
                                __LINE__, __FILE__, errno, strerror(errno));
                goto fd_finish;
            }
            ret = 0;

            if (op == OP_READ) {
                map_base = mmap(0,
                                MAP_SIZE, PROT_READ,
                                MAP_SHARED,
                                fd, addr & ~MAP_MASK);
            } else {
                map_base = mmap(0,
                                MAP_SIZE, PROT_WRITE,
                                MAP_SHARED,
                                fd, addr & ~MAP_MASK);
            }
            if ((void *) -1 == map_base) {
                ret = -1;
                utils_printf("mmap /dev/mem failed\n");
                utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",
                                __LINE__, __FILE__, errno, strerror(errno));
                goto fd_finish;
            }

            utils_printf("map_base over 4k = [%p].\n", map_base);
        }
        virt_addr = map_base + (addr & MAP_MASK);
        if (op == OP_WRITE) {
            *(volatile uint64_t*)virt_addr = buffer[i];
        } else {
            buffer[i] = *(volatile uint64_t*)virt_addr;
        }
        addr += 4, offset += 4;
    }

    ret = munmap(map_base, MAP_SIZE);
    if (-1 == ret) {
        utils_printf("munmap failed\n");
        utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",
                        __LINE__, __FILE__, errno, strerror(errno));
    }

    *io_len = i;

fd_finish:
    close(fd);
finish:
    return ret;
}

int main(int argc, char **argv)
{
    int ret = 0;
    uint64_t buffer[BUFFER_SIZE];
    size_t len = 0;
    size_t i = 0;
    intptr_t addr = 0;

    if (argc != 4) {
        ret = -1;
        devmem_usage();
        goto finish;
    }

    // check input args addr
    addr = (uint64_t)strtoul(argv[2], 0, 0);
    len = (uint64_t)strtoul(argv[3], 0, 0);
    utils_printf("Target phy addr : %lx\n", addr);
    utils_printf("Target len: %ld\n", len);
    memset(buffer, 0, len);

    // check input args op
    if (strcmp(argv[1], "read") == 0) {
        utils_printf("OP: Read data\n");
        ret = devmem_io(addr, buffer, &len, OP_READ);
        if (0 != ret) {
            utils_printf("read error!\n");
            utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",
                            __LINE__, __FILE__, errno, strerror(errno));
            goto finish;
        }
        for (i = 0; i < len; i ++) {
            utils_printf("address = 0x%08lx, data = 0x%08lx\n",
                         (addr + i * 4), buffer[i]);
        }
    } else if (strcmp(argv[1], "write") == 0) {
        utils_printf("OP: Write data\n");
        for (i = 0; i < len; i++) {
            buffer[i] = i;
        }
        ret = devmem_io(addr, buffer, &len, OP_WRITE);
        if (0 != ret) {
            utils_printf("write error!\n");
            utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n",
                            __LINE__, __FILE__, errno, strerror(errno));
            goto finish;
        }
    } else {
        utils_printf("error command type\n");
        devmem_usage();
    }

finish:
    return ret;
}