/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** md5.h
 ** Contains information for a static class that computes md5 hashes.
 **
 ** Author: James Holland - jholland@usgs.gov
 ** -------------------------------------------------------------------------*/
#ifndef _MD5_HPP
#define _MD5_HPP

#include <array>
#include <vector>
#include <string>

class Md5 {
public:
    /**
     * The static hashing function. It accepts a reference to a vector of
     * bytes and returns the hash as a byte array.
     *
     * @param message A reference to a constant vector of bytes.
     */
    static std::array<std::uint8_t, 16> hash (
            std::vector<std::uint8_t> const & message
            );

    /**
     * The static hashing function. It accepts a reference to a string
     * and returns the hash as a byte array.
     *
     * @param message A reference to a constant string.
     */
    static std::array<std::uint8_t, 16> hash (
            std::string const & message
            );

private:
	static void updateHash(
            std::array<std::uint32_t, 16> const & chunk,
			std::array<std::uint32_t, 4> & hash);

	static inline  std::uint32_t leftRotate(
            std::uint32_t number,
			std::uint32_t shift);

	static inline std::uint32_t step1(
            std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
			std::uint32_t k, std::array<std::uint32_t, 16> const & chunk
            );

	static inline std::uint32_t step2(
            std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
			std::uint32_t k, std::array<std::uint32_t, 16> const & chunk
            );

	static inline std::uint32_t step3(
            std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
			std::uint32_t k, std::array<std::uint32_t, 16> const & chunk
            );

	static inline std::uint32_t step4(
            std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
			std::uint32_t k, std::array<std::uint32_t, 16> const & chunk
            );

	static constexpr std::array<std::uint32_t, 64> SINE_CONSTANTS {{
			0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf,
			0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af,
			0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e,
			0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
			0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6,
			0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
			0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122,
			0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
			0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039,
			0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97,
			0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d,
			0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
			0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 }};

	static constexpr std::array<std::uint32_t, 64> SHIFT_AMOUNTS {{ 7, 12, 17,
			22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 5, 9, 14, 20, 5, 9,
			14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 4, 11, 16, 23, 4, 11, 16, 23, 4,
			11, 16, 23, 4, 11, 16, 23, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15,
			21, 6, 10, 15, 21 }};

	// Little Endian form of 0x0123456789abcdeffedcba9876543210
	static constexpr std::array<std::uint32_t, 4> BUFFER_INIT {{ 0x67452301,
			0xefcdab89, 0x98badcfe, 0x10325476 }};
};

#endif
