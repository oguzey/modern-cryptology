#include "RoundKey.h"
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>



struct round_key {
	uint16_t keys[AMOUNT_SUBKEYS];
	int current_subkey;
};


RoundKey* round_key_create(void)
{
	RoundKey *rkey = malloc(sizeof(RoundKey));
	rkey->current_subkey = 0;
	round_key_generate_new(rkey);
	return rkey;
}

void round_key_destroy(RoundKey *a_rkey)
{
	if (a_rkey) {
		free(a_rkey);
	}

}

void round_key_generate_new(RoundKey *a_rkey)
{
	assert(a_rkey);

	int i = 0;

	srand(time(NULL));
	for(i = 0; i < AMOUNT_SUBKEYS; ++i) {
		a_rkey->keys[i] = UINT16_C(rand()) % UINT_LEAST16_MAX;
	}
}

void round_key_set_key(RoundKey *a_rkey, uint16_t *a_new_keys)
{
	assert(a_rkey);
	assert(a_new_keys);

	int i = 0;

	for(i = 0; i < AMOUNT_SUBKEYS; ++i) {
		a_rkey->keys[i] = a_new_keys[i];
	}

}

void round_key_reset_using(RoundKey *a_rkey)
{
	assert(a_rkey);

	a_rkey->current_subkey = 0;
}

uint16_t round_key_next(RoundKey *a_rkey)
{
	assert(a_rkey);
	assert(a_rkey->current_subkey >= 0 && a_rkey->current_subkey < AMOUNT_SUBKEYS);

	return a_rkey->keys[(a_rkey->current_subkey)++];
}
