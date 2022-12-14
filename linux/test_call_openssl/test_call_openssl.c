#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static const char test_array[256] = {
        0x13,0x9e,0x26,0xaf,0xc5,0x72,0x44,0xbc,0x6d,0x78,0x50,0x66,0x2f,0x66,0x8f,0x8e,
        0x4f,0xa0,0x34,0x03,0x7c,0x72,0x20,0x46,0x12,0xbd,0x7b,0x74,0xbe,0xf7,0x38,0x11,
        0x9d,0xe6,0x03,0x8b,0x4f,0xcc,0x42,0x16,0xa7,0xd0,0x8d,0x9b,0x7d,0x9e,0x10,0x36,
        0x9d,0x38,0x35,0x76,0x31,0xa3,0x23,0x54,0x74,0x1e,0xc1,0x16,0xd3,0x18,0x59,0xfb,
        0xdf,0x2a,0x7b,0xe4,0x2b,0x0d,0xd3,0xa0,0xb2,0x0f,0x9a,0xe9,0x7e,0xc8,0x0e,0x1e,
        0x13,0xea,0x6a,0x20,0xb9,0x0f,0x68,0x06,0xe4,0xb7,0xad,0x7d,0xca,0xb1,0x83,0x10,
        0xa2,0x9e,0x9f,0xd8,0x39,0x47,0x8f,0x7a,0x8f,0x70,0x57,0xbd,0x90,0xef,0xec,0x5f,
        0xb4,0x1e,0x62,0xe8,0xd6,0x35,0xc5,0x87,0x52,0x27,0x94,0xcd,0xe4,0x53,0xeb,0xb5,
        0xa2,0xd9,0x28,0x61,0x34,0x43,0xab,0x5a,0xd9,0xc9,0x38,0x50,0xba,0x35,0x0c,0x4c,
        0x8c,0xd7,0xc7,0xaa,0x79,0x2f,0x0d,0x00,0x27,0x90,0x08,0x04,0x50,0xbe,0xd8,0x7b,
        0x92,0x08,0x9b,0xb7,0x6d,0xe1,0xc2,0x2e,0x13,0xce,0xbd,0xa3,0xd5,0x22,0x46,0xb9,
        0x27,0xee,0x57,0x28,0xe8,0x7a,0x27,0x2f,0x3c,0x2e,0xbe,0xd0,0xfa,0xd1,0xad,0x91,
        0xb4,0xb4,0x2c,0x43,0xce,0x45,0xc0,0xdb,0x73,0x44,0x65,0x57,0xb4,0x4c,0xda,0x4a,
        0xbb,0xc6,0x25,0x8c,0x5f,0x7a,0x24,0xd5,0xac,0xc4,0xc3,0x0a,0xcb,0x7d,0x7e,0x48,
        0x04,0x40,0xba,0x33,0x79,0xca,0x50,0x1d,0x4f,0xf5,0xbd,0x8e,0x4b,0xee,0xef,0xe6,
        0xcd,0x00,0xe7,0x3f,0xd9,0x65,0xd0,0xcc,0x60,0x27,0x80,0x7b,0xe3,0x7e,0x07,0x85
};
#define CMD_MAX_BYTES (2048U)
static char __cmd[CMD_MAX_BYTES] = { 0 };
// openssl genrsa -out pri.pem 4096
// openssl rsa -in pri.pem -pubout > pub.pem
#define PRIVATE_KEY "pri.pem"
#define PUBLIC_KEY "pub.pem"
#define CACHE_FILE_INPUT_NAME "cache.bin"
#define CACHE_FILE_SIGN_NAME "cache_sign_file.bin"
#define OPENSSL_CMD_DGST_SHA256 "-sha256"
#define OPENSSL_CMD_DGST_SHA3_256 "-sha3-256"

#define INVOKE_OPENSSL_SIGN_CMD(__DIGEST, __PRIV_KEY, __IN_FILE, __OUT_FILE)    \
    do {                                                                        \
        sprintf(__cmd, "openssl dgst %s -sign %s -out %s %s",                   \
                (__DIGEST),                                                     \
                (__PRIV_KEY),                                                   \
                (__OUT_FILE),                                                   \
                (__IN_FILE));                                                   \
        ret = system(__cmd);                                                    \
        if (ret < 0) {                                                          \
            printf("invoke $ %s failed, ret = %d\n", __cmd, ret);               \
            goto finish;                                                        \
        }                                                                       \
    } while(0)

#define INVOKE_OPENSSL_VERIFY_CMD(__DIGEST, __PUB_KEY, __IN_FILE, __OUT_FILE)   \
    do {                                                                        \
        sprintf(__cmd, "openssl dgst %s -verify %s -signature %s %s",           \
                (__DIGEST),                                                     \
                (__PUB_KEY),                                                    \
                (__OUT_FILE),                                                   \
                (__IN_FILE));                                                   \
        ret = system(__cmd);                                                    \
        if (ret < 0) {                                                          \
            printf("invoke $ %s failed, ret = %d\n", __cmd, ret);               \
            goto finish;                                                        \
        }                                                                       \
    } while(0)

static int32_t porting_gen_bin_file(const uint8_t *data,
                                    size_t len,
                                    const char *out_file)
{
    FILE *fp = NULL;
    int32_t ret = 0;
    int32_t write_sz = 0;

    if ((NULL == data) ||
        (NULL == out_file)) {
        ret = -1;
        printf("input *data or *out_file is NULL\n");
        goto finish;
    }

    if (0 == len) {
        printf("write len is 0, no action\n");
        goto finish;
    }

    fp = fopen(out_file, "w");
    if (NULL == fp) {
        ret = -1;
        printf("open file is failed\n");
        goto finish;
    }

    write_sz = fwrite(data, sizeof(uint8_t), len, fp);
    if (write_sz != len) {
        ret = -1;
        printf("write bytes failed, return write size= %d\n", write_sz);
        goto free_fd;
    }

free_fd:
    if (fp != NULL)
        fclose(fp);
finish:
    return ret;
}

static int32_t porting_sign_with_rsa4096_file(const char *in_file,
                                              const char *priv_key,
                                              const char *sign_out_file)
{
    int32_t ret = 0;

    if ((NULL == in_file) ||
        (NULL == priv_key) ||
        (NULL == sign_out_file)) {
        ret = -1;
        printf("input params error \n");
        goto finish;
    }

    INVOKE_OPENSSL_SIGN_CMD(OPENSSL_CMD_DGST_SHA3_256,
                            priv_key,
                            in_file,
                            sign_out_file);

finish:
    return ret;
}

static int32_t porting_verify_with_rsa4096_file(const char *in_file,
                                                const char *pub_key,
                                                const char *signed_file)
{
    int32_t ret = 0;
    char cmd[1024] = { 0 };

    if ((NULL == in_file) ||
        (NULL == pub_key) ||
        (NULL == signed_file)) {
        ret = -1;
        printf("input params error \n");
        goto finish;
    }

    INVOKE_OPENSSL_VERIFY_CMD(OPENSSL_CMD_DGST_SHA3_256,
                              pub_key,
                              in_file,
                              signed_file);

finish:
    return ret;
}

static int32_t porting_read_sign_file(const char *signed_file,
                                      uint8_t *array_out,
                                      size_t *sz)
{
    FILE *fp = NULL;
    int32_t ret = 0;
    size_t read_sz = 0;
    struct stat info;

    if ((NULL == signed_file) ||
        (NULL == array_out) ||
        (NULL == sz)) {
        ret = -1;
        printf("input params error \n");
        goto finish;
    }

    ret = stat(signed_file, &info);
    if (ret < 0) {
        printf("file stat is error\n");
        goto finish;
    }

    if ((size_t)info.st_size > *sz) {
        ret = -1;
        printf("the buffer size inputted is not enough. %lu != %lu\n",
               (size_t)info.st_size,
               *sz);
        goto finish;
    }

    fp = fopen(signed_file, "r");
    if (NULL == fp) {
        ret = -1;
        printf("read %s file failed\n", signed_file);
        goto finish;
    }

    read_sz = fread((void *)array_out, sizeof(uint8_t), (size_t)info.st_size, fp);
    if (read_sz != (size_t)info.st_size) {
        ret = -1;
        *sz = read_sz;
        printf("read file size is not matched! %lu != %lu\n", read_sz, (size_t)info.st_size);
        goto free_fp;
    }

    *sz = read_sz;

free_fp:
    if (fp != NULL) {
        fclose(fp);
    }
finish:
    return ret;
}

int32_t main(void)
{
    int32_t ret = 0;
    uint8_t buffer[2048];
    size_t out_len = 0;

    ret = porting_gen_bin_file(test_array, 256, CACHE_FILE_INPUT_NAME);
    if (ret != 0) {
        printf("gen file failed\n");
        goto finish;
    }

    ret = porting_sign_with_rsa4096_file(CACHE_FILE_INPUT_NAME,
                                         PRIVATE_KEY,
                                         CACHE_FILE_SIGN_NAME);
    if (ret != 0) {
        printf("sign file failed\n");
        goto finish;
    }

    ret = porting_verify_with_rsa4096_file(CACHE_FILE_INPUT_NAME,
                                           PUBLIC_KEY,
                                           CACHE_FILE_SIGN_NAME);
    if (ret != 0) {
        printf("verify file failed\n");
        goto finish;
    }

    out_len = 2048;
    ret = porting_read_sign_file(CACHE_FILE_SIGN_NAME,
                                 buffer,
                                 &out_len);
    if (ret != 0) {
        printf("read sign file failed\n");
        goto finish;
    }

    printf("done\n");

finish:
    return ret;
}