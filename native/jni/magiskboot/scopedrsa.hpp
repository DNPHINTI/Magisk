/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>

#include "android_pubkey.h"

class ScopedRSA {
 public:
  ScopedRSA(const void* privkey) {
    BIO *b = BIO_new_mem_buf(privkey, -1);
    if (b == NULL) {
        return;
    }
    rsa_ = PEM_read_bio_RSAPrivateKey(b, nullptr, nullptr, nullptr);
  }

  ScopedRSA(const uint8_t* key_buffer, size_t size) {
    android_pubkey_decode(key_buffer, size, &rsa_);
  }

  ~ScopedRSA() {
    if (rsa_) {
      RSA_free(rsa_);
    }
  }

  bool encode(uint8_t* key_buffer, size_t size) {
    return android_pubkey_encode(rsa_, key_buffer, size);
  }

  // PKCS #1 v1.5 signature using SHA256. Returns true on success.
  bool sign(const uint8_t* digest, size_t digest_size, uint8_t signature[]) {
    uint32_t signature_length = 0;
    return RSA_sign(NID_sha256, digest, digest_size, signature, &signature_length, rsa_);
  }

  bool verify(const uint8_t* digest, size_t digest_size, const uint8_t signature[], size_t signature_length) {
    return RSA_verify(NID_sha256, digest, digest_size, signature, signature_length, rsa_);
  }

  int size() {
    return RSA_size(rsa_);
  }

 private:
  RSA* rsa_;
};
