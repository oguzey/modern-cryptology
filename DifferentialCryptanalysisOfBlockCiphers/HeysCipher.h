#ifndef HEYSCIPHER_H
#define HEYSCIPHER_H

#include "RoundKey.h"
#include <stddef.h>
#include <vector>

//typedef struct heys_cipher HeysCipher;
//
//typedef enum {
//	UNDEFINED = 0,
//	ENCRYPT,
//	DECRYPT
//} Action;
//
//HeysCipher* heys_cipher_create(RoundKey *a_round_key);
//
//void heys_cipher_destroy(HeysCipher *a_heys_cipher);
//
//void heys_cipher_run(HeysCipher *a_heys_cipher, Action a_action, uint16_t *a_input_data
//		,size_t a_size, uint16_t **a_output_data);
//
//
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

private:
	static const std::array<std::array<uint16_t, 16>, 2> sboxs;
	RoundKey *round_key;
	inline uint16_t encrypt(uint16_t a_input);
	inline uint16_t decrypt(uint16_t a_input);
	inline uint16_t addition_with_key(uint16_t a_block, uint16_t a_key);
	inline uint16_t substitution_by_sbox(Action a_action, uint16_t a_block);
	inline uint16_t permutation_of_bits(uint16_t a_block);


};




#endif // HEYSCIPHER_H
