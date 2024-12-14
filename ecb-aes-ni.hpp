#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <wmmintrin.h>  // For AES-NI instructions

// AES block and key sizes
constexpr size_t BLOCK_SIZE = 16;		// 128-bit block size
constexpr size_t AES_KEY_SIZE = 64;		// 512-bit key size
constexpr size_t ROUNDS = 16;			// Number of encryption rounds

// AES-NI: Add Round Key using AES-NI instructions
void AddRoundKey_AESNI( __m128i& block, const __m128i& round_key ) {
	block = _mm_xor_si128( block, round_key );
}

// AES-NI: Substitute bytes using AES-NI instructions (SubBytes)
void SubBytes_AESNI( __m128i& block ) {
	block = _mm_aesenc_si128( block, block );
}

// AES-NI: Inverse Substitute bytes (InvSubBytes)
void InvSubBytes_AESNI( __m128i& block ) {
	block = _mm_aesenclast_si128( block, block );
}

// AES-NI: Encrypt a block using AES-NI
void Encrypt_AESNI( std::array<uint8_t, BLOCK_SIZE>& block, const std::vector<std::array<uint8_t, BLOCK_SIZE>>& round_keys ) {
	__m128i block_state = _mm_loadu_si128( reinterpret_cast<__m128i*>( block.data() ) );
	__m128i round_key;

	// Initial round key addition
	round_key = _mm_loadu_si128( reinterpret_cast<const __m128i*>( round_keys[ 0 ].data() ) );
	AddRoundKey_AESNI( block_state, round_key );

	// 15 rounds of encryption
	for ( size_t round = 1; round < ROUNDS; ++round ) {
		round_key = _mm_loadu_si128( reinterpret_cast<const __m128i*>( round_keys[ round ].data() ) );
		SubBytes_AESNI( block_state );  // Substitute bytes (SubBytes)
		block_state = _mm_aesenc_si128( block_state, round_key );  // AES encryption
	}

	// Final round
	round_key = _mm_loadu_si128( reinterpret_cast<const __m128i*>( round_keys[ ROUNDS - 1 ].data() ) );
	SubBytes_AESNI( block_state );  // Substitute bytes (SubBytes)
	block_state = _mm_aesenclast_si128( block_state, round_key );  // Final encryption step

	_mm_storeu_si128( reinterpret_cast<__m128i*>( block.data() ), block_state );  // Store result in block
}

// AES-NI: Decrypt a block using AES-NI
void Decrypt_AESNI( std::array<uint8_t, BLOCK_SIZE>& block, const std::vector<std::array<uint8_t, BLOCK_SIZE>>& round_keys ) {
	__m128i block_state = _mm_loadu_si128( reinterpret_cast<__m128i*>( block.data() ) );
	__m128i round_key;

	// Initial round key addition (inverse)
	round_key = _mm_loadu_si128( reinterpret_cast<const __m128i*>( round_keys[ 0 ].data() ) );
	AddRoundKey_AESNI( block_state, round_key );

	// 15 rounds of decryption
	for ( size_t round = 1; round < ROUNDS; ++round ) {
		round_key = _mm_loadu_si128( reinterpret_cast<const __m128i*>( round_keys[ round ].data() ) );
		InvSubBytes_AESNI( block_state );  // Inverse Substitute bytes (InvSubBytes)
		block_state = _mm_aesdec_si128( block_state, round_key );  // AES decryption
	}

	// Final round
	round_key = _mm_loadu_si128( reinterpret_cast<const __m128i*>( round_keys[ ROUNDS - 1 ].data() ) );
	InvSubBytes_AESNI( block_state );  // Inverse Substitute bytes (InvSubBytes)
	block_state = _mm_aesdeclast_si128( block_state, round_key );  // Final decryption step

	_mm_storeu_si128( reinterpret_cast<__m128i*>(block.data()), block_state);  // Store result in block
}

// Key expansion using AES-NI (key schedule)
std::vector<std::array<uint8_t, BLOCK_SIZE>> KeyExpansion_AESNI( const std::array<uint8_t, AES_KEY_SIZE>& key ) {
	std::vector<std::array<uint8_t, BLOCK_SIZE>> round_keys( ROUNDS );
	__m128i key_schedule;

	// Process 512-bit key in chunks of 128-bits per round
	for ( size_t i = 0; i < ROUNDS; i++ ) {
		key_schedule = _mm_loadu_si128( reinterpret_cast<const __m128i*>( key.data() + i * 16 ) ); // Load 128 bits of key
		round_keys[ i ] = *reinterpret_cast<std::array<uint8_t, BLOCK_SIZE>*>( &key_schedule );

		// Perform AES key expansion (simplified for demonstration)
		if ( i < ROUNDS - 1 ) {
			key_schedule = _mm_aeskeygenassist_si128( key_schedule, 0 );
		}
	}

	return round_keys;
}


