#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "HeysCipher.h"
#include "../CommonLib/logger.h"


#define SUB_BLOCKS		4
#define BITS_IN_SUB_BLOCK    4
#define AMOUNT_OF_ROUNDS	6

struct heys_cipher {
	RoundKey *round_key;
};

/**
 * x - number
 * i - position second bit to swap
 * k - position first bit to swap
 * NOTE. i > k !!!
 */
#define SWAP_BITS(x, i, k) 							\
		/* set only i and k bits */					\
		((x & (1<<i)) >> (i-k)) | ((x & (1<<k)) << (i-k)) |		\
		/* clear i and k bits in x */					\
		(x & ~(1<<i) & ~(1<<k))

//static uint16_t _s_sbox[] = {0xF, 0x6, 0x5, 0x8, 0xE, 0xB, 0xA, 0x4, 0xC, 0x0, 0x3, 0x7, 0x2, 0x9, 0x1, 0xD};
//
//static inline void addition_with_key(uint16_t *a_block, uint16_t a_key);
//static inline void substitution_by_sbox(uint16_t *a_block);
//static inline void permutation_of_bits(uint16_t *a_block);
//static inline void encrypt(RoundKey *a_round_key, uint16_t *a_block);
//static inline void decrypt(RoundKey *a_round_key, uint16_t *a_block);
//
//typedef void(*action_worker)(RoundKey *a_round_key, uint16_t *a_block);
//
//static action_worker do_action[] = {encrypt, decrypt};

/*****************************************************************
 **************** PUBLIC FUNCTIONS *******************************
 *****************************************************************/

HeysCipher* heys_cipher_create(RoundKey *a_round_key)
{
	HeysCipher *cipher = malloc(sizeof(HeysCipher));
	cipher->round_key = a_round_key;
	return cipher;
}


void heys_cipher_destroy(HeysCipher *a_heys_cipher)
{
	if (a_heys_cipher) {
		free(a_heys_cipher);
	}
}

void heys_cipher_run(HeysCipher *a_heys_cipher, Action a_action, uint16_t *a_input_data
		,size_t a_size, uint16_t **a_output_data)
{
	assert(a_input_data);

	int index = 0;
	uint16_t *output = malloc(sizeof(uint16_t) * a_size);
	memset(output, 0, sizeof(uint16_t) * a_size);
	memcpy(output, a_input_data, sizeof(uint16_t) * a_size);

	for (index = 0; index < a_size; ++index) {
		do_action[a_action](a_heys_cipher->round_key, &output[index]);
	}

	*a_output_data = output;
}

/*****************************************************************
 **************** PRIVATE FUNCTIONS ******************************
 *****************************************************************/

static inline void addition_with_key(uint16_t *a_block, uint16_t a_key)
{
	*a_block ^= a_key;
}

static inline void substitution_by_sbox(uint16_t *a_block)
{
	int pos = 0;
	int i = 0;
	for(i = 0; i < SUB_BLOCKS; ++i) {
		pos = (*a_block >> 4 * i) & 0xF;
		*a_block = _s_sbox[pos];
	}
}

static inline void permutation_of_bits(uint16_t *a_block)
{
	int pos_i = 0;
	int pos_j = 0;
	int i = 0
		,k = 0;

	for(i = 0; i < SUB_BLOCKS; ++i) {
		for(k = 0; k < BITS_IN_SUB_BLOCK; ++k) {

			pos_i = i * BITS_IN_SUB_BLOCK + k;
			pos_j = k * BITS_IN_SUB_BLOCK + i;
			if (pos_i > pos_j) {
				*a_block = (uint16_t)(SWAP_BITS((*a_block), pos_i, pos_j));
			} else {
				*a_block = (uint16_t)(SWAP_BITS((*a_block), pos_j, pos_i));
			}
		}
	}
}

static inline void encrypt(RoundKey *a_round_key, uint16_t *a_block)
{
	assert(a_round_key);

	int i = 0;

	round_key_reset_using_to_start(a_round_key);

	for (i = 0; i < AMOUNT_OF_ROUNDS; ++i) {
		addition_with_key(a_block, round_key_next_subkey(a_round_key));
		substitution_by_sbox(a_block);
		permutation_of_bits(a_block);
	}
	addition_with_key(a_block, round_key_next_subkey(a_round_key));
}

static inline void decrypt(RoundKey *a_round_key, uint16_t *a_block)
{
	assert(a_round_key);

	int i = 0;

	round_key_reset_using_to_end(a_round_key);

	addition_with_key(a_block, round_key_next_subkey(a_round_key));

	for (i = 0; i < AMOUNT_OF_ROUNDS; ++i) {
		permutation_of_bits(a_block);
		substitution_by_sbox(a_block);
		addition_with_key(a_block, round_key_next_subkey(a_round_key));
	}

}


namespace mc {

	HeysCipher::HeysCipher(RoundKey& a_round_key) : round_key(a_round_key) {
		logger.debug("HeysCipher constructor was called");
	}

	HeysCipher::~HeysCipher() { }

	void HeysCipher::run(Action a_action, std::vector<uint16_t> &a_input, std::vector<uint16_t> &a_output) {
		
	}

	uint16_t HeysCipher::addition_with_key(uint16_t a_block, uint16_t a_key) {
		return a_block ^ a_key;
	}

	uint16_t HeysCipher::substitution_by_sbox(uint16_t a_block) {

		uint16_t output = 0;
		int pos = 0;

		for (int i = 0; i < amount_subblocks; ++i) {
			pos = (a_block >> 4 * i) & 0xF;
			output += sbox[pos];
			output <<= 4 * i;
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

		uint16_t output = 0;
		uint16_t subkey = 0;

		round_key.set_direction(FORWARD);
		round_key.reset_position();

		for (int round = 0; round < amount_rounds; ++round) {
			subkey = round_key.next_subkey();
			output = addition_with_key(a_input, subkey);
			output = substitution_by_sbox(output);
			output = permutation_of_bits(output);
		}

		subkey = round_key.next_subkey();
		output = addition_with_key(output, subkey);
		return output;
	}

	inline uint16_t HeysCipher::decrypt(uint16_t a_input) {
		uint16_t output = 0;
		uint16_t subkey = 0;

		round_key.set_direction(BACKWARD);
		round_key.reset_position();

		subkey = round_key.next_subkey();
		output = addition_with_key(a_input, subkey);

		for (int round = 0; round < amount_rounds; ++round) {
			output = permutation_of_bits(output);
			output = substitution_by_sbox(output);
			subkey = round_key.next_subkey();
			output = addition_with_key(output, subkey);
		}

		return output;
	}
}