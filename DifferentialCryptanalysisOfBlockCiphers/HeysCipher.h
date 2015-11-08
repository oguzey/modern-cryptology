#ifndef HEYSCIPHER_H
#define HEYSCIPHER_H

#include "RoundKey.h"
#include <stddef.h>

typedef struct heys_cipher HeysCipher;

HeysCipher* heys_cipher_create(RoundKey *a_round_key);

void heys_cipher_destroy(HeysCipher *a_heys_cipher);

void heys_cipher_encrypt(HeysCipher *a_heys_cipher, uint16_t *a_input_data
		,size_t a_size, uint16_t **a_output_data);

#endif // HEYSCIPHER_H
