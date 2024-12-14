#include <iostream>
#include <array>
#include <random>
#include <iomanip>

#include "ecb-aes.hpp"

//You don't need this, you can hard code a key or use your own key generator
// Function to generate a random key and return it as std::array
std::array<uint8_t, AES_KEY_SIZE> generateRandomKey() {
	std::array<uint8_t, AES_KEY_SIZE> key{};
	std::random_device rd;                // Random device for entropy
	std::mt19937 generator( rd() );         // Mersenne Twister engine
	std::uniform_int_distribution<uint8_t> distribution( 0, 255 );

	for ( size_t i = 0; i < AES_KEY_SIZE; ++i ) {
		key[ i ] = distribution( generator );
	}

	return key;
}

void printKey( const std::array<uint8_t, AES_KEY_SIZE>& key ) {
	for ( size_t i = 0; i < key.size(); ++i ) {
		std::cout << std::hex << std::setw( 2 ) << std::setfill( '0' ) << static_cast<int>( key[ i ] );

		if ( i % 16 == 15 ) {
			std::cout << std::endl; // Format in rows of 16 bytes
		} else {
			std::cout << " ";
		}
	}
}

int main() {
	// Example key and plaintext
	std::array<uint8_t, AES_KEY_SIZE> key = generateRandomKey( AES_KEY_SIZE ); // Initialize with your 512-bit key
	Block plaintext = { 0x00 }; // Initialize with your 128-bit plaintext
	Block ciphertext, decryptedtext;

	// Key expansion
	auto round_keys = KeyExpansion( key );

	// Encrypt
	ciphertext = plaintext;
	Encrypt( ciphertext, round_keys );

	// Decrypt
	decryptedtext = ciphertext;
	Decrypt( decryptedtext, round_keys );

	// Display results
	printKey( key );
	std::cout << "Plaintext: ";
	for ( auto byte : plaintext ) std::cout << std::hex << ( int )byte << " ";
	std::cout << "\nCiphertext: ";
	for ( auto byte : ciphertext ) std::cout << std::hex << ( int )byte << " ";
	std::cout << "\nDecrypted: ";
	for ( auto byte : decryptedtext ) std::cout << std::hex << ( int )byte << " ";
	std::cout << "\n";

	return 0;
}


