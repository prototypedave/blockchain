/*
 * kbc.h
 *
 *  Created on: Nov 20, 2024
 *      Author: david
 */

#ifndef INET_APPLICATIONS_KBC_KBC_H_
#define INET_APPLICATIONS_KBC_KBC_H_

#include <openssl/rand.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace inet {

struct VehicleData {
    std::string vehicleId;
    const char *service;
    std::string vehicleIp;
    double lifetime;
    unsigned char * vehicleKey;

    VehicleData(std::string Id, const char *service, std::string ip, double lt, unsigned char * key):
        vehicleId(Id), service(service), vehicleIp(ip), lifetime(lt), vehicleKey(key) {}
};

struct RSUData {
    int RSUId;
    std::string RSUIp;
    std::string RSUname;
    unsigned char secretKey[16];

    // Constructor
    RSUData(int Id, const std::string& ip, const std::string& name)
        : RSUId(Id), RSUIp(ip), RSUname(name) {}

    // Copy Constructor
    RSUData(const RSUData& other)
        : RSUId(other.RSUId), RSUIp(other.RSUIp), RSUname(other.RSUname) {
        std::copy(std::begin(other.secretKey), std::end(other.secretKey), std::begin(secretKey));
    }

    // Assignment Operator
    RSUData& operator=(const RSUData& other) {
        if (this != &other) {
            RSUId = other.RSUId;
            RSUIp = other.RSUIp;
            RSUname = other.RSUname;
            std::copy(std::begin(other.secretKey), std::end(other.secretKey), std::begin(secretKey));
        }
        return *this;
    }
};


class KBC {
protected:
    unsigned char iv[16] = {
        0x2a, 0x7f, 0xd3, 0x4c, 0x9b, 0x12, 0x58, 0x6d,
        0x1e, 0xa0, 0xc5, 0x37, 0xf8, 0x99, 0x23, 0x4b
    };

    std::string encrypt(const std::string &plainText, const unsigned char *key) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

        std::vector<unsigned char> cipherText(plainText.size() + AES_BLOCK_SIZE);
        int len = 0, cipherTextLen = 0;

        EVP_EncryptUpdate(ctx, cipherText.data(), &len, (unsigned char*)plainText.c_str(), plainText.size());
        cipherTextLen = len;

        EVP_EncryptFinal_ex(ctx, cipherText.data() + len, &len);
        cipherTextLen += len;

        EVP_CIPHER_CTX_free(ctx);

        return bin2hex(cipherText.data(), cipherTextLen);
    }

    // Decryption function
    std::string decrypt(const std::string &cipherText, const unsigned char *key) {
        std::vector<unsigned char> binCipherText = hex2bin(cipherText);

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);

        std::vector<unsigned char> plainText(binCipherText.size());
        int len = 0, plainTextLen = 0;

        EVP_DecryptUpdate(ctx, plainText.data(), &len, binCipherText.data(), binCipherText.size());
        plainTextLen = len;

        EVP_DecryptFinal_ex(ctx, plainText.data() + len, &len);
        plainTextLen += len;

        EVP_CIPHER_CTX_free(ctx);

        return std::string((char*)plainText.data(), plainTextLen);
    }

    std::string bin2hex(const unsigned char* bin, size_t len) {
        std::ostringstream oss;
        for (size_t i = 0; i < len; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)bin[i];
        }
        return oss.str();
    }

    // Convert a hex string to a binary buffer
    std::vector<unsigned char> hex2bin(const std::string &hex) {
        std::vector<unsigned char> bin(hex.size() / 2);
        for (size_t i = 0; i < hex.size(); i += 2) {
            std::istringstream iss(hex.substr(i, 2));
            int byte;
            iss >> std::hex >> byte;
            bin[i / 2] = static_cast<unsigned char>(byte);
        }
        return bin;
    }

    // ensure you free memory when using this function
    bool generateSecretKey(unsigned char* buffer, size_t size) {
        if (size < 16) {
            return false;
        }
        if (!RAND_bytes(buffer, 16)) {
            return false;
        }
        return true;
    }
};
}


#endif /* INET_APPLICATIONS_KBC_KBC_H_ */
