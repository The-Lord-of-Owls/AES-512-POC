#include <iostream>
#include "aes.hpp"

int main() {
	// Example key and plaintext
	Key key = { 0x00 }; // Initialize with your 512-bit key
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
	std::cout << "Plaintext: ";
	for ( auto byte : plaintext ) std::cout << std::hex << ( int )byte << " ";
	std::cout << "\nCiphertext: ";
	for ( auto byte : ciphertext ) std::cout << std::hex << ( int )byte << " ";
	std::cout << "\nDecrypted: ";
	for ( auto byte : decryptedtext ) std::cout << std::hex << ( int )byte << " ";
	std::cout << "\n";

	return 0;
}


