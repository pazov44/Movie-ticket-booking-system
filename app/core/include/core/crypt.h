#pragma once

/// @brief Size of the salt buffer (in bytes) used for hashing.
#define SALT_BYTES 16

namespace Crypt
{
    /**
     * @brief Calculates a SHA-256 hash of the input string, optionally using a salt.
     *
     * @param src The input string to hash.
     * @param srcSalt Pointer to a byte array used as salt (can be nullptr to skip salting).
     * @param dwLenSalt Length of the salt in bytes.
     * @param dst Output string to store the resulting hash in hexadecimal format.
     */
    void CalcHash(const std::string& src, byte srcSalt[], DWORD dwLenSalt, std::string& dst);

    /**
     * @brief Fills a buffer with cryptographically secure random bytes.
     *
     * @param dst Destination byte array to fill with random values.
     * @param dwLen Length of the destination buffer.
     */
    void GenRandom(byte dst[], DWORD dwLen);

    /**
     * @brief Converts a byte array to a hexadecimal string.
     *
     * @param src Input byte array.
     * @param dwLenSrc Length of the byte array.
     * @param hex Output string containing the hexadecimal representation.
     */
    void ByteToHex(const byte src[], DWORD dwLenSrc, std::string& hex);

    /**
     * @brief Converts a hexadecimal string back to a byte array.
     *
     * @param src Input hexadecimal string.
     * @param dwLenDst Expected length of the output byte array.
     * @param dst Output byte array to store converted bytes.
     */
    void HexToByte(const std::string& src, DWORD dwLenDst, byte dst[]);
}