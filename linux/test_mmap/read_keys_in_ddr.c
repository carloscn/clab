#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "mbedtls/aes.h"

// How to compile it:
// aarch64-linux-gnu-gcc read_keys_in_ddr.c
// -L{your mbedtls path}/mbedtls/out/lib
// -I{your mbedtls path}/mbedtls/out/include
// -lmbedcrypto -o read_key.elf
// How to add key slot on LUKS:
// $ cryptsetup luksAddKey /dev/sda2 /root/enc.key

#define DEBUG 1
#define LUKS_KEY_ADDR (0x70000000)
#define LUKS_IV_ADDR (0x700000D0)
#define LUKS_KEY_SIZE 32u
#define LUKS_IV_SIZE 12u
#define LUKS_PASS_SIZE_ASCII 32u
#define LUKS_PASS_SIZE_BIN 16u
#define LUKS_PASS_PATH ("/mnt/sd-mmcblk0p1/LUKS.PASS")
#define LUKS_PASS_NODE ("/root/enc.key")

#define utils_printf        \
  printf("[devmem_tool] "); \
  printf
#define utils_fprintf                \
  fprintf(stderr, "[devmem_tool] "); \
  fprintf
#define UTILS_CHECK_RET(err)                              \
  do {                                                    \
    if (ret != 0) {                                       \
      utils_printf("Operation failed: err= 0x%x\n", err); \
      ret = -1;                                           \
      goto finish;                                        \
    }                                                     \
  } while (0)
#define UTILS_CHECK_PTR(p)               \
  do {                                   \
    if (NULL == (p)) {                   \
      utils_printf("pointer is NULL\n"); \
      ret = -1;                          \
      goto finish;                       \
    }                                    \
  } while (0)

#define MAP_SIZE (4096UL)        // Page size
#define MAP_MASK (MAP_SIZE - 1)  // MAP_MASK = 0XFFF
#define BUFFER_SIZE (MAP_SIZE + MAP_SIZE)

typedef enum operation_t { OP_READ = 0, OP_WRITE } IO_TYPE_T;

static void devmem_print_hex(const uint8_t *buffer, size_t size) {
  size_t i = 0;

  for (i = 0; i < size; i++) {
    printf("%02X", buffer[i]);
  }
  printf("\n\r");
}

static int32_t devmem_io(intptr_t physical_addr, uint8_t *buffer,
                         size_t *io_len, IO_TYPE_T op) {
  int32_t ret = 0;
  int32_t fd = 0;
  size_t i = 0;
  size_t offset = 0;
  size_t len = 0;
  intptr_t addr = physical_addr;
  void *map_base = NULL;
  void *virt_addr = NULL;

  if (NULL == buffer || NULL == io_len || 0 == *io_len || op >= 2) {
    ret = -1;
    utils_printf("%s %s %d, input error\n", __FILE__, __func__, __LINE__);
    goto finish;
  }

  len = *io_len;

  fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd < 0) {
    ret = -1;
    utils_printf("open /dev/mem failed\n");
    utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__,
                  __FILE__, errno, strerror(errno));
    goto finish;
  }

  if (op == OP_READ) {
    map_base = mmap(0, MAP_SIZE, PROT_READ, MAP_SHARED, fd, addr & ~MAP_MASK);
  } else {
    map_base = mmap(0, MAP_SIZE, PROT_WRITE, MAP_SHARED, fd, addr & ~MAP_MASK);
  }
  if ((void *)-1 == map_base) {
    ret = -1;
    utils_printf("mmap /dev/mem failed\n");
    utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__,
                  __FILE__, errno, strerror(errno));
    goto fd_finish;
  }

  for (i = 0; i < len; i++) {
    // turn the page
    if (offset >= MAP_MASK) {
      offset = 0;
      ret = munmap(map_base, MAP_SIZE);
      if (-1 == ret) {
        utils_printf("munmap /dev/mem failed\n");
        utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__,
                      __FILE__, errno, strerror(errno));
        goto fd_finish;
      }
      ret = 0;

      if (op == OP_READ) {
        map_base =
            mmap(0, MAP_SIZE, PROT_READ, MAP_SHARED, fd, addr & ~MAP_MASK);
      } else {
        map_base =
            mmap(0, MAP_SIZE, PROT_WRITE, MAP_SHARED, fd, addr & ~MAP_MASK);
      }
      if ((void *)-1 == map_base) {
        ret = -1;
        utils_printf("mmap /dev/mem failed\n");
        utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__,
                      __FILE__, errno, strerror(errno));
        goto fd_finish;
      }

      utils_printf("map_base over 4k = [%p].\n", map_base);
    }
    virt_addr = map_base + (addr & MAP_MASK);
    if (op == OP_WRITE) {
      *(volatile uint8_t *)virt_addr = buffer[i];
    } else {
      buffer[i] = *(volatile uint8_t *)virt_addr;
    }
    addr += 1, offset += 1;
  }

  ret = munmap(map_base, MAP_SIZE);
  if (-1 == ret) {
    utils_printf("munmap failed\n");
    utils_fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", __LINE__,
                  __FILE__, errno, strerror(errno));
  }

  *io_len = i;

fd_finish:
  close(fd);
finish:
  return ret;
}

static int32_t devmem_read_luks_key(intptr_t key_addr, uint8_t *key_out,
                                    size_t key_bytes) {
  int32_t ret = 0;
  size_t len = 0;

  UTILS_CHECK_PTR(key_out);

  if (key_bytes == 0) {
    goto finish;
  }

  len = key_bytes;
  ret = devmem_io((intptr_t)key_addr, key_out, &len, OP_READ);
  UTILS_CHECK_RET(ret);

  utils_printf("The key is : \n");
  devmem_print_hex(key_out, key_bytes);

finish:
  return ret;
}

static int32_t devmem_read_luks_iv(intptr_t iv_addr, uint8_t *iv_out,
                                   size_t iv_bytes) {
  int32_t ret = 0;
  size_t len = 0;
  size_t i = 0;

  UTILS_CHECK_PTR(iv_out);

  if (iv_bytes == 0) {
    goto finish;
  }

  len = iv_bytes;
  ret = devmem_io((intptr_t)iv_addr, iv_out, &len, OP_READ);
  if (ret != 0) {
    utils_printf("[ERR] read LUKS_IV error on 0x%lX!\n", (intptr_t)iv_addr);
    goto finish;
  }
#if DEBUG
  utils_printf("The IV is : \n");
  devmem_print_hex(iv_out, iv_bytes);
#endif /* DEBUG */

finish:
  return ret;
}

static int32_t devmem_aes_encrypt(uint8_t *buffer_io, const uint8_t *key,
                                  size_t key_len) {
  int32_t ret = 0;
  uint8_t *buffer_out = NULL;
  size_t len = 16;
  mbedtls_aes_context aes;

  UTILS_CHECK_PTR(buffer_io);
  UTILS_CHECK_PTR(key);

  mbedtls_aes_init(&aes);

  ret = mbedtls_aes_setkey_enc(&aes, (const unsigned char *)key, key_len * 8);
  if (0 != ret) {
    utils_printf("mbedtls_aes_setkey_enc failed\n");
    goto finish;
  }

  buffer_out = (uint8_t *)malloc(len);
  if (NULL == buffer_out) {
    ret = -1;
    utils_printf("buffer_out malloc failed\n");
    goto finish;
  }
  ret = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT,
                              (const unsigned char *)buffer_io,
                              (unsigned char *)buffer_out);
  if (0 != ret) {
    utils_printf("mbedtls_aes_crypt_ecb failed\n");
    goto finish;
  }

  memcpy(buffer_io, buffer_out, len);

#if DEBUG
  utils_printf("The encrypted content is : \n");
  devmem_print_hex(buffer_io, len);
#endif /* DEBUG */

finish:
  if (buffer_out != NULL) {
    free(buffer_out);
  }
  mbedtls_aes_free(&aes);
  return ret;
}

static int32_t devmem_aes_decrypt(uint8_t *buffer_io, const uint8_t *key,
                                  size_t key_len) {
  int32_t ret = 0;
  uint8_t *buffer_out = NULL;
  size_t len = 16;
  mbedtls_aes_context aes;

  UTILS_CHECK_PTR(buffer_io);
  UTILS_CHECK_PTR(key);

  mbedtls_aes_init(&aes);

  ret = mbedtls_aes_setkey_dec(&aes, (const unsigned char *)key, key_len * 8);
  if (0 != ret) {
    utils_printf("mbedtls_aes_setkey_dec failed\n");
    goto finish;
  }

  buffer_out = (uint8_t *)malloc(len);
  if (NULL == buffer_out) {
    ret = -1;
    utils_printf("buffer_out malloc failed\n");
    goto finish;
  }
  ret = mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT,
                              (const unsigned char *)buffer_io,
                              (unsigned char *)buffer_out);
  if (0 != ret) {
    utils_printf("mbedtls_aes_crypt_ecb failed\n");
    goto finish;
  }

  memcpy(buffer_io, buffer_out, len);

#if DEBUG
  utils_printf("The decrypted content is :");
  devmem_print_hex(buffer_io, len);
#endif /* DEBUG */

finish:
  if (buffer_out != NULL) {
    free(buffer_out);
  }
  mbedtls_aes_free(&aes);
  return ret;
}

static int32_t utils_convert_char_to_nibble(uint8_t inchar, uint8_t *num) {
  int32_t ret = 0;

  UTILS_CHECK_PTR(num);

  /* Convert the char to nibble */
  if ((inchar >= (uint8_t)'0') && (inchar <= (uint8_t)'9')) {
    *num = inchar - (uint8_t)'0';
  } else if ((inchar >= (uint8_t)'a') && (inchar <= (uint8_t)'f')) {
    *num = inchar - (uint8_t)'a' + 10U;
  } else if ((inchar >= (uint8_t)'A') && (inchar <= (uint8_t)'F')) {
    *num = inchar - (uint8_t)'A' + 10U;
  } else {
    ret = -1;
  }

finish:
  return ret;
}

static int32_t utils_convert_str_to_be(const char *str, uint8_t *buf,
                                       size_t len) {
  int32_t ret = -1;
  size_t c_len = 0;
  uint8_t lower_nibble, upper_nibble;

  UTILS_CHECK_PTR(str);
  UTILS_CHECK_PTR(buf);

  /* Len has to be multiple of 2 */
  if ((0 == len) || (len % 2 == 1)) {
    goto finish;
  }

  c_len = 0;
  while (c_len < len) {
    if (utils_convert_char_to_nibble(str[c_len], &upper_nibble) == 0) {
      if (utils_convert_char_to_nibble(str[c_len + 1], &lower_nibble) == 0) {
        buf[c_len / 2] = (upper_nibble << 4) | lower_nibble;
      } else {
        goto finish;
      }
    } else {
      goto finish;
    }
    c_len += 2;
  }
  ret = 0;
finish:
  return ret;
}

static int32_t devmem_read_luks_passphrase(uint8_t *buffer_io,
                                           size_t *file_len) {
  int32_t ret = 0;
  FILE *p = NULL;
  size_t read_len = 0;
  char buffer[LUKS_PASS_SIZE_ASCII + 1];

  UTILS_CHECK_PTR(buffer_io);
  UTILS_CHECK_PTR(file_len);

  memset(buffer, '\0', LUKS_PASS_SIZE_ASCII + 1);

  p = fopen(LUKS_PASS_PATH, "r");
  if (NULL == p) {
    ret = -1;
    utils_printf("open %s failed\n", LUKS_PASS_PATH);
    goto finish;
  }

  read_len = fread(buffer, sizeof(char), LUKS_PASS_SIZE_ASCII, p);
  if (read_len != LUKS_PASS_SIZE_ASCII) {
    ret = -1;
    utils_printf("open %s failed\n", LUKS_PASS_PATH);
    goto finish;
  }

#if DEBUG
  utils_printf("The file read ascii buffer is : ");
  devmem_print_hex((const uint8_t *)buffer, read_len);
  utils_printf("string: %s\n", buffer);
#endif /* DEBUG */

  ret = utils_convert_str_to_be(buffer, buffer_io, read_len);
  if (ret != 0) {
    utils_printf("utils_convert_str_to_be failed\n");
    goto finish;
  }

  *file_len = read_len / 2;

#if DEBUG
  utils_printf("The file read binary buffer is : ");
  devmem_print_hex((const uint8_t *)buffer_io, *file_len);
#endif /* DEBUG */

finish:
  if (p != NULL) {
    fclose(p);
  }
  return ret;
}

static int32_t devmem_write_luks_key(uint8_t *buffer_io, size_t len) {
  int32_t ret = 0;
  size_t write_len = 0;
  FILE *p = NULL;

  UTILS_CHECK_PTR(buffer_io);

  p = fopen(LUKS_PASS_NODE, "wb");
  if (NULL == p) {
    ret = -1;
    utils_printf("open %s failed\n", LUKS_PASS_NODE);
    goto finish;
  }

  write_len = fwrite(buffer_io, sizeof(uint8_t), len, p);
  if (write_len != len) {
    ret = -1;
    utils_printf("open %s failed\n", LUKS_PASS_PATH);
    goto finish;
  }

finish:
  if (p != NULL) {
    fclose(p);
  }
  return ret;
}

int main(int argc, char **argv) {
  int ret = 0;
  size_t len = 16;
  size_t i = 0;

  uint8_t luks_key[LUKS_KEY_SIZE] = {0};
  uint8_t luks_iv[LUKS_IV_SIZE] = {0};
  uint8_t decrypted_buffer[128] = {0};

  utils_printf("call read luks key:\n");
  ret = devmem_read_luks_key(LUKS_KEY_ADDR, luks_key, LUKS_KEY_SIZE);
  if (ret != 0) {
    utils_printf("[ERR] devmem_read_luks_key\n");
    goto finish;
  }

#if 0
  utils_printf("call read luks iv:\n");
  ret = devmem_read_luks_iv(LUKS_IV_ADDR, luks_iv, LUKS_IV_SIZE);
  if (ret != 0) {
    utils_printf("[ERR] devmem_read_iv_key\n");
    goto finish;
  }

  // using the aes encryption
  memset(decrypted_buffer, 0, len);
  for (i = 0; i < 9; i ++) {
    decrypted_buffer[i] = i + '0';
  }
  utils_printf("the input buffer is :\n");
  devmem_print_hex(decrypted_buffer, len);

  len = 16;
  utils_printf("call mbedtls aes encrypt:\n");
  ret = devmem_aes_encrypt(decrypted_buffer, luks_key, LUKS_KEY_SIZE);
  if (ret != 0) {
    utils_printf("[ERR] devmem_gcm_encrypt\n");
    goto finish;
  }
#endif

  // read encrypted passphrase from SD card.
  len = LUKS_PASS_SIZE_ASCII;
  ret = devmem_read_luks_passphrase(decrypted_buffer, &len);
  if (ret != 0) {
    utils_printf("[ERR] devmem_read_luks_passphrase\n");
    goto finish;
  }

  utils_printf("call mbedtls aes decrypt.");
  ret = devmem_aes_decrypt(decrypted_buffer, luks_key, LUKS_KEY_SIZE);
  if (ret != 0) {
    utils_printf("[ERR] devmem_gcm_decrypt\n");
    goto finish;
  }

#if DEBUG
  utils_printf("Finally, the passphrase is [%s].\n", (char *)decrypted_buffer);
#endif /* DEBUG */

  ret = devmem_write_luks_key(decrypted_buffer, LUKS_PASS_SIZE_BIN);
  if (ret != 0) {
    utils_printf("[ERR] devmem_write_luks_key\n");
    goto finish;
  }

finish:
  return ret;
}