#ifndef HEYSCIPHER_H
#define HEYSCIPHER_H

#include "RoundKey.h"
#include <stddef.h>

typedef struct heys_cipher HeysCipher;

typedef enum {
	ENCRYPT = 0,
	DECRYPT
} Action;

HeysCipher* heys_cipher_create(RoundKey *a_round_key);

void heys_cipher_destroy(HeysCipher *a_heys_cipher);

void heys_cipher_run(HeysCipher *a_heys_cipher, Action a_action, uint16_t *a_input_data
		,size_t a_size, uint16_t **a_output_data);

#endif // HEYSCIPHER_H
