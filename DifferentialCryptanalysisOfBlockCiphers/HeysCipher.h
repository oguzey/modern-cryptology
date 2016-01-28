#ifndef HEYSCIPHER_H
#define HEYSCIPHER_H

#include "RoundKey.h"
#include <stddef.h>
#include <vector>

enum Action {
	UNDEFINED = -1,
	ENCRYPT,
	DECRYPT
};

class HeysCipher {

public:
	const int amount_subblocks = 4;
	const int amount_bit_in_subblock = 4;
	const int amount_rounds = 6;

	HeysCipher(RoundKey *a_round_key);
	~HeysCipher();
	void run(Action a_action, std::vector<uint16_t> &a_input, std::vector<uint16_t> &a_output);
	void calc_dp_for_sbox(std::array<std::array<double, 16>, 16> &dp_sbox);
	void calc_dp_table(std::array<std::array<double, UINT16_MAX + 1>, UINT16_MAX + 1> &dp_cipher);

private:
	std::array<std::array<uint16_t, 16>, 2> sboxs;
	RoundKey *round_key;
	inline uint16_t encrypt(uint16_t a_input);
	inline uint16_t decrypt(uint16_t a_input);
	inline uint16_t addition_with_key(uint16_t a_block, uint16_t a_key);
	inline uint16_t substitution_by_sbox(Action a_action, uint16_t a_block);
	inline uint16_t permutation_of_bits(uint16_t a_block);

};


#endif // HEYSCIPHER_H
