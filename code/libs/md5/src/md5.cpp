/* ---------------------------------------------------------------------------
 ** This software is in the public domain, furnished "as is", without technical
 ** support, and with no warranty, express or implied, as to its usefulness for
 ** any purpose.
 **
 ** md5.cpp
 ** Implementation of the md5 hashing algorithm specified in RF 1321.
 ** See https://tools.ietf.org/html/rfc1321
 **
 **
 **
 ** Author: James Holland - jholland@usgs.gov
 ** -------------------------------------------------------------------------*/
#include "md5.h"
#include <array>
#include <cstdint>

constexpr std::array<std::uint32_t, 64> Md5::SINE_CONSTANTS;
constexpr std::array<std::uint32_t, 64> Md5::SHIFT_AMOUNTS;
constexpr std::array<std::uint32_t, 4> Md5::BUFFER_INIT;

std::array<std::uint8_t, 16> Md5::hash(
		std::vector<std::uint8_t> const & message) {

	// The byte form of the hash array.
	std::array < std::uint8_t, 16> hash = {};

	// The storage of our hash as it is built.
	std::array<std::uint32_t, 4> buffer = Md5::BUFFER_INIT;

	// The 512 bit chunk of message to update the hash with.
	std::array<std::uint32_t, 16> chunk = {};
	// The current byte in the message.
	int messageByteIndex = 0;
	// The index we are filling in chunk.
	int chunkIndex = 0;
	// Indicates which byte we are populating the in the chunk.
	int bytePosition = 0;

	// Form chunks from the message.
	while ( messageByteIndex < message.size() ) {

		// Indexing starts at 0.
		chunk[chunkIndex] |= (message[messageByteIndex] << bytePosition);

		// Update indexing for next step.
		messageByteIndex++;
		chunkIndex = (messageByteIndex % 64) / 4;
		bytePosition = (messageByteIndex % 4) * 8;

		// If true, we just finished a chunk. Update immediately.
		if (chunkIndex == 0 && bytePosition == 0) {
			Md5::updateHash(chunk, buffer);
			chunk.fill(0);
		}
	}

	// We finished the message, next byte is always 128 (10000000b)
	chunk[chunkIndex] |= (128 << bytePosition);

	// Update indexing to check for padding requirements.
	messageByteIndex++;
	chunkIndex = (messageByteIndex % 64) / 4;
	bytePosition = (messageByteIndex % 4) * 8;

	// The 0 padding that follows the first bit is already in place from the
	// fill(0).

	// Check if we missed the 448 bit mark
	// If so update the Hash and reset chunk to 0s
	if ((chunkIndex == 14 && bytePosition != 0) || chunkIndex == 15) {
		Md5::updateHash(chunk, buffer);
		chunk.fill(0);
	}

	// Add formatted message size to last 64 bits.
	// We assume that no more than 64 bits of address space is used.
	std::uint64_t messageSize = message.size() * 8;

	chunk[14] = static_cast<std::uint32_t>(messageSize);
	chunk[15] = static_cast<std::uint32_t>(messageSize >> 32);

	Md5::updateHash(chunk, buffer);

	hash[0] = static_cast<std::uint8_t>(buffer[0]);
	hash[1] = static_cast<std::uint8_t>(buffer[0] >> 8);
	hash[2] = static_cast<std::uint8_t>(buffer[0] >> 16);
	hash[3] = static_cast<std::uint8_t>(buffer[0] >> 24);

	hash[4] = static_cast<std::uint8_t>(buffer[1]);
	hash[5] = static_cast<std::uint8_t>(buffer[1] >> 8);
	hash[6] = static_cast<std::uint8_t>(buffer[1] >> 16);
	hash[7] = static_cast<std::uint8_t>(buffer[1] >> 24);

	hash[8] = static_cast<std::uint8_t>(buffer[2]);
	hash[9] = static_cast<std::uint8_t>(buffer[2] >> 8);
	hash[10] = static_cast<std::uint8_t>(buffer[2] >> 16);
	hash[11] = static_cast<std::uint8_t>(buffer[2] >> 24);

	hash[12] = static_cast<std::uint8_t>(buffer[3]);
	hash[13] = static_cast<std::uint8_t>(buffer[3] >> 8);
	hash[14] = static_cast<std::uint8_t>(buffer[3] >> 16);
	hash[15] = static_cast<std::uint8_t>(buffer[3] >> 24);
	return hash;
}

void Md5::updateHash(std::array<std::uint32_t, 16> const & chunk,
		             std::array<std::uint32_t, 4> & hash) {

	std::uint32_t A = hash[0];
	std::uint32_t B = hash[1];
	std::uint32_t C = hash[2];
	std::uint32_t D = hash[3];

	A = Md5::step1(A, B, C, D, 0, chunk);
	D = Md5::step1(D, A, B, C, 1, chunk);
	C = Md5::step1(C, D, A, B, 2, chunk);
	B = Md5::step1(B, C, D, A, 3, chunk);
	A = Md5::step1(A, B, C, D, 4, chunk);
	D = Md5::step1(D, A, B, C, 5, chunk);
	C = Md5::step1(C, D, A, B, 6, chunk);
	B = Md5::step1(B, C, D, A, 7, chunk);
	A = Md5::step1(A, B, C, D, 8, chunk);
	D = Md5::step1(D, A, B, C, 9, chunk);
	C = Md5::step1(C, D, A, B, 10, chunk);
	B = Md5::step1(B, C, D, A, 11, chunk);
	A = Md5::step1(A, B, C, D, 12, chunk);
	D = Md5::step1(D, A, B, C, 13, chunk);
	C = Md5::step1(C, D, A, B, 14, chunk);
	B = Md5::step1(B, C, D, A, 15, chunk);

	A = Md5::step2(A, B, C, D, 0, chunk);
	D = Md5::step2(D, A, B, C, 1, chunk);
	C = Md5::step2(C, D, A, B, 2, chunk);
	B = Md5::step2(B, C, D, A, 3, chunk);
	A = Md5::step2(A, B, C, D, 4, chunk);
	D = Md5::step2(D, A, B, C, 5, chunk);
	C = Md5::step2(C, D, A, B, 6, chunk);
	B = Md5::step2(B, C, D, A, 7, chunk);
	A = Md5::step2(A, B, C, D, 8, chunk);
	D = Md5::step2(D, A, B, C, 9, chunk);
	C = Md5::step2(C, D, A, B, 10, chunk);
	B = Md5::step2(B, C, D, A, 11, chunk);
	A = Md5::step2(A, B, C, D, 12, chunk);
	D = Md5::step2(D, A, B, C, 13, chunk);
	C = Md5::step2(C, D, A, B, 14, chunk);
	B = Md5::step2(B, C, D, A, 15, chunk);

	A = Md5::step3(A, B, C, D, 0, chunk);
	D = Md5::step3(D, A, B, C, 1, chunk);
	C = Md5::step3(C, D, A, B, 2, chunk);
	B = Md5::step3(B, C, D, A, 3, chunk);
	A = Md5::step3(A, B, C, D, 4, chunk);
	D = Md5::step3(D, A, B, C, 5, chunk);
	C = Md5::step3(C, D, A, B, 6, chunk);
	B = Md5::step3(B, C, D, A, 7, chunk);
	A = Md5::step3(A, B, C, D, 8, chunk);
	D = Md5::step3(D, A, B, C, 9, chunk);
	C = Md5::step3(C, D, A, B, 10, chunk);
	B = Md5::step3(B, C, D, A, 11, chunk);
	A = Md5::step3(A, B, C, D, 12, chunk);
	D = Md5::step3(D, A, B, C, 13, chunk);
	C = Md5::step3(C, D, A, B, 14, chunk);
	B = Md5::step3(B, C, D, A, 15, chunk);

	A = Md5::step4(A, B, C, D, 0, chunk);
	D = Md5::step4(D, A, B, C, 1, chunk);
	C = Md5::step4(C, D, A, B, 2, chunk);
	B = Md5::step4(B, C, D, A, 3, chunk);
	A = Md5::step4(A, B, C, D, 4, chunk);
	D = Md5::step4(D, A, B, C, 5, chunk);
	C = Md5::step4(C, D, A, B, 6, chunk);
	B = Md5::step4(B, C, D, A, 7, chunk);
	A = Md5::step4(A, B, C, D, 8, chunk);
	D = Md5::step4(D, A, B, C, 9, chunk);
	C = Md5::step4(C, D, A, B, 10, chunk);
	B = Md5::step4(B, C, D, A, 11, chunk);
	A = Md5::step4(A, B, C, D, 12, chunk);
	D = Md5::step4(D, A, B, C, 13, chunk);
	C = Md5::step4(C, D, A, B, 14, chunk);
	B = Md5::step4(B, C, D, A, 15, chunk);

	hash[0] += A;
	hash[1] += B;
	hash[2] += C;
	hash[3] += D;
}

std::array<std::uint8_t, 16> Md5::hash(std::string const & message){
    std::vector<std::uint8_t> byteMessage(message.begin(), message.end());
    return Md5::hash(byteMessage);
}

inline std::uint32_t Md5::leftRotate(std::uint32_t number,
		std::uint32_t shift) {
	return (number << shift) | (number >> (32 - shift));
}

inline std::uint32_t Md5::step1(std::uint32_t a, std::uint32_t b,
        std::uint32_t c, std::uint32_t d, std::uint32_t k,
        std::array<std::uint32_t, 16> const & chunk)
{
    return b + Md5::leftRotate
        (
         (
          a +
          ((b & c) | ((~b) & d)) +
          chunk[k] +
          SINE_CONSTANTS[k]
         ),
         SHIFT_AMOUNTS[k]
        );
}

inline std::uint32_t Md5::step2(std::uint32_t a, std::uint32_t b,
        std::uint32_t c, std::uint32_t d, std::uint32_t k,
        std::array<std::uint32_t, 16> const & chunk)
{
    k += 16;
    return b + Md5::leftRotate
        (
         (
          a +
          ((b & d) | (c & (~d))) +
          chunk[(5 * k + 1) % 16] +
          SINE_CONSTANTS[k]
         ),
         SHIFT_AMOUNTS[k]
        );
}

inline std::uint32_t Md5::step3(std::uint32_t a, std::uint32_t b,
        std::uint32_t c, std::uint32_t d, std::uint32_t k,
        std::array<std::uint32_t, 16> const & chunk)
{
    k += 32;
    return b + Md5::leftRotate
        (
         (
          a +
          (b ^ c ^ d) +
          chunk[(3 * k + 5) % 16]
          + SINE_CONSTANTS[k]
         ),
         SHIFT_AMOUNTS[k]
        );
}

inline std::uint32_t Md5::step4(std::uint32_t a, std::uint32_t b,
        std::uint32_t c, std::uint32_t d, std::uint32_t k,
        std::array<std::uint32_t, 16> const & chunk)
{
    k += 48;
    return b + Md5::leftRotate
        (
         (
          a +
          (c ^ (b | (~d))) +
          chunk[(7 * k) % 16] +
          SINE_CONSTANTS[k]
         ),
         SHIFT_AMOUNTS[k]
        );
}
