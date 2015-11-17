#include "HeysCipher.h"
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define SUBBLOCKS		4
#define BITS_IN_SUBBLOCK	4
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

static int _s_sbox[] = {0xF, 0x6, 0x5, 0x8, 0xE, 0xB, 0xA, 0x4, 0xC, 0x0, 0x3, 0x7, 0x2, 0x9, 0x1, 0xD};

static inline void addition_with_key(uint16_t *a_block, uint16_t a_key);
static inline void substitution_by_sbox(uint16_t *a_block);
static inline void permutation_of_bits(uint16_t *a_block);
static inline void encrypt(RoundKey *a_round_key, uint16_t *a_block);
static inline void decrypt(RoundKey *a_round_key, uint16_t *a_block);

typedef void(*action_worker)(RoundKey *a_round_key, uint16_t *a_block);

static action_worker do_action[] = {encrypt, decrypt};

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
	for(i = 0; i < SUBBLOCKS; ++i) {
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

	for(i = 0; i < SUBBLOCKS; ++i) {
		for(k = 0; k < BITS_IN_SUBBLOCK; ++k) {

			pos_i = i * BITS_IN_SUBBLOCK + k;
			pos_j = k * BITS_IN_SUBBLOCK + i;
			if (pos_i > pos_j) {
				*a_block = SWAP_BITS((*a_block), pos_i, pos_j);
			} else {
				*a_block = SWAP_BITS((*a_block), pos_j, pos_i);
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
