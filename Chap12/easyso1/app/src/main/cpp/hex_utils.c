//
// Created by ting on 2019-09-17.
//

#include <stdlib.h>
#include <string.h>
#include "hex_utils.h"

char *hex_encode(const unsigned char *input, size_t len) {
    if (input == NULL || len == 0) return NULL;

    char *out = malloc(len * 2 + 1);
    for (int i = 0; i < len; i++) {
        out[i * 2] = "0123456789abcdef"[input[i] >> 4];
        out[i * 2 + 1] = "0123456789abcdef"[input[i] & 0x0F];
    }
    out[len * 2] = '\0';
    return out;
}

int hex_hex2Char(const char hex, char *out) {
    if (out == NULL) return 0;

    if (hex >= '0' && hex <= '9') {
        *out = (char) (hex - '0');
    } else if (hex >= 'A' && hex <= 'F') {
        *out = (char) (hex - 'A' + 10);
    } else if (hex >= 'a' && hex <= 'f') {
        *out = (char) (hex - 'a' + 10);
    } else {
        return 0;
    }
    return 1;
}

unsigned char *hex_decode(const char *hex) {
    if (hex == NULL || *hex == '\0') return NULL;

    size_t len = strlen(hex);
    if (len % 2 != 0) return NULL;
    len /= 2;

    unsigned char *out = malloc(len + 1);
    memset(out, 'A', len);

    char b1;
    char b2;
    for (int i = 0; i < len; i++) {
        if (!hex_hex2Char(hex[i * 2], &b1) || !hex_hex2Char(hex[i * 2 + 1], &b2)) {
            return 0;
        }
        (out)[i] = (unsigned char) ((b1 << 4) | b2);
    }
    out[len] = '\0';
    return out;
}