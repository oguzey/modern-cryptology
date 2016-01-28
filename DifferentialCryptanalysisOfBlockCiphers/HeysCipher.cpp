#include <assert.h>
#include "HeysCipher.h"
#include "../CommonLib/logger.h"

#define SUB_BLOCKS		4
#define BITS_IN_SUB_BLOCK    4
#define AMOUNT_OF_ROUNDS	6



#define SWAP_BITS(x, i, j) 													\
({																			\
	unsigned int tmp##i##j = ((x >> i) ^ (x >> j)) & ((1U << 1) - 1);			\
	x ^ ((tmp##i##j << i) | (tmp##i##j << j));									\
})

static const std::array<uint16_t, 16> sbox =
		{0xF, 0x6, 0x5, 0x8, 0xE, 0xB, 0xA, 0x4, 0xC, 0x0, 0x3, 0x7, 0x2, 0x9, 0x1, 0xD};

static const std::array<uint16_t, 16> sbox_inverse =
		{0x9, 0xE, 0xC, 0xA, 0x7, 0x2, 0x1, 0xB, 0x3 , 0xD, 0x6, 0x5, 0x8, 0xF, 0x4, 0x0};



HeysCipher::HeysCipher(RoundKey *a_round_key)
{
	round_key = a_round_key;
	sboxs = {sbox, sbox_inverse};
	logger.debug("HeysCipher created with RoundKey object");
}

HeysCipher::~HeysCipher() { }

void HeysCipher::run(Action a_action, std::vector<uint16_t> &a_input, std::vector<uint16_t> &a_output)
{
	assert(a_action == ENCRYPT || a_action == DECRYPT);
	if (a_action == ENCRYPT) {
		for (int i = 0; i < a_input.size(); ++i) {
			a_output.push_back(encrypt(a_input[i]));
		}
	} else {
		for (int i = 0; i < a_input.size(); ++i) {
			a_output.push_back(decrypt(a_input[i]));
		}
	}
}

uint16_t HeysCipher::addition_with_key(uint16_t a_block, uint16_t a_key) {
	return a_block ^ a_key;
}

uint16_t HeysCipher::substitution_by_sbox(Action a_action, uint16_t a_block) {

	assert(a_action == ENCRYPT || a_action == DECRYPT);
	uint16_t output = 0;
	uint16_t tmp = 0;
	int pos = 0;
	logger.debug("substitution_by_sbox starting with {:X}", a_block);
	for (int i = 0; i < amount_subblocks; ++i) {
		pos = (a_block >> 4 * i) & 0xF;
		tmp = HeysCipher::sboxs[a_action][pos];
		output += (tmp << (4 * i));
		logger.debug("step {}:  pos = {:X},  tmp = {:X}, output = {:X}", i, pos, tmp, output);
	}
	return output;
}

inline uint16_t HeysCipher::permutation_of_bits(uint16_t a_block) {
	uint16_t output = a_block;
	uint16_t pos_i = 0;
	uint16_t pos_j = 0;

	for (int i = 0; i < amount_subblocks; ++i) {
		for (int j = i + 1; j < amount_bit_in_subblock; ++j) {

			pos_i = (uint16_t) (i * amount_bit_in_subblock + j);
			pos_j = (uint16_t) (j * amount_bit_in_subblock + i);
			output = (uint16_t) (SWAP_BITS(output, pos_i, pos_j));
		}
	}
	return output;
}

inline uint16_t HeysCipher::encrypt(uint16_t a_input) {

	uint16_t output = a_input;
	uint16_t subkey = 0;

	round_key->set_direction(FORWARD);
	round_key->reset_position();

	for (int round = 0; round < amount_rounds; ++round) {
		subkey = round_key->next_subkey();
		output = addition_with_key(output, subkey);
		//logger.debug("addition_with_key key {:X}  output {:X} ", subkey, output);
		output = substitution_by_sbox(ENCRYPT, output);
		//logger.debug("substitution_by_sbox\t{:X} ", output);
		output = permutation_of_bits(output);
		//logger.debug("permutation_of_bits\t{:X} ", output);
	}

	subkey = round_key->next_subkey();
	output = addition_with_key(output, subkey);
	//logger.debug("addition_with_key key {:X}  output {:X} ", subkey, output);

	//logger.info("Final result encryption for {:X} is output {:X}", a_input, output);
	return output;
}

inline uint16_t HeysCipher::decrypt(uint16_t a_input) {
	uint16_t output = 0;
	uint16_t subkey = 0;

	round_key->set_direction(BACKWARD);
	round_key->reset_position();
	//logger.debug("input block for DECRYPT {:X}", a_input);
	subkey = round_key->next_subkey();
	output = addition_with_key(a_input, subkey);
	//logger.debug("fist step subkey {:X} output {:X}", subkey, output);


	for (int round = 0; round < amount_rounds; ++round) {
		subkey = round_key->next_subkey();

		//logger.debug("Round {} ", round);
		output = permutation_of_bits(output);
		//logger.debug("permutation_of_bits {:X} ", output);
		output = substitution_by_sbox(DECRYPT, output);
		//logger.debug("substitution_by_sbox {:X} ", output);
		output = addition_with_key(output, subkey);
		//logger.debug("addition_with_key key {:X}  output {:X} ", subkey, output);
	}
	//logger.debug("Final result decryption for {:X} is output {:X} ", a_input, output);
	return output;
}

void HeysCipher::calc_dp_for_sbox(std::array<std::array<double, 16>, 16> &dp_sbox) {
	std::array<uint16_t, 16> &sbox = sboxs[ENCRYPT];
	for (int alfa = 0; alfa < 16; ++alfa) {
		for (int beta = 0; beta < 16; ++beta) {
			dp_sbox[alfa][beta] = 0.0;
			for (int x = 0; x < 16; ++x) {
				if (sbox[x ^ alfa] == (sbox[x] ^ beta)) {
					dp_sbox[alfa][beta] += 0.0625;
				}
			}
		}
	}
}

void HeysCipher::calc_dp_table(std::array<std::array<double, UINT16_MAX + 1>, UINT16_MAX + 1> &dp_cipher) {

	std::array<std::array<double, 16>, 16> dp_sbox;
	calc_dp_for_sbox(dp_sbox);

	for (uint16_t alfa = 0; alfa <= UINT16_MAX; ++alfa) {
		for (uint16_t beta = 0; beta <= UINT16_MAX; ++beta) {

			int beta_s = permutation_of_bits(beta);
			int alfa_i = alfa & 0xF;
			int beta_s_i = beta_s & 0xF;
			dp_cipher[alfa][beta] = dp_sbox[alfa_i][beta_s_i];
			logger.info("take alfa_i = {:X}, beta_i = {:X}", alfa_i, beta_s_i);

			for (int i = 1; i < 4; ++i) {
				alfa_i = (beta_s >> 4 * i) & 0xF;
				beta_s_i = (beta_s >> 4 * i) & 0xF;
				dp_cipher[alfa][beta] *= dp_sbox[alfa_i][beta_s_i];
			}
			logger.info("dp_cipher[{:X}][{:X}] = {}", alfa, beta, dp_cipher[alfa][beta]);
		}
	}
}
