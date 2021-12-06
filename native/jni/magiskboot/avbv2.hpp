#pragma once

#define AVB2_INVALID     (1 << 1)
#define AVB2_UNSUPPORTED (1 << 2)

std::string testkey_rsa4096 = ""\
    "-----BEGIN RSA PRIVATE KEY-----\n"\
    "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\n"\
    "-----END RSA PRIVATE KEY-----\n";

int avbv2_verify_sign(const char *image, bool rw);
std::string mem_to_hexstring(const uint8_t* data, size_t len);
