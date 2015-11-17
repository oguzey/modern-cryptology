#include "RoundKey.h"
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

enum direction {
	UNKNOWN,
	FORWARD,
	BACKWARD
};

struct round_key {
	uint16_t keys[AMOUNT_SUBKEYS];
	int current_subkey;
	enum direction path;
};


RoundKey* round_key_create(uint16_t *a_start_value)
{
	RoundKey *rkey = malloc(sizeof(RoundKey));
	rkey->current_subkey = 0;
	if (a_start_value) {
		round_key_set_key(rkey, a_start_value);
	} else {
		round_key_generate_new(rkey);
	}
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

	a_rkey->path = UNKNOWN;

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

	a_rkey->path = UNKNOWN;

	for(i = 0; i < AMOUNT_SUBKEYS; ++i) {
		a_rkey->keys[i] = a_new_keys[i];
	}

}

void round_key_reset_using_to_start(RoundKey *a_rkey)
{
	assert(a_rkey);
	a_rkey->path = FORWARD;
	a_rkey->current_subkey = 0;
}

void round_key_reset_using_to_end(RoundKey *a_rkey)
{
	assert(a_rkey);
	a_rkey->path = BACKWARD;
	a_rkey->current_subkey = AMOUNT_SUBKEYS - 1;
}

uint16_t round_key_next_subkey(RoundKey *a_rkey)
{
	assert(a_rkey);
	assert(UNKNOWN != a_rkey->path);
	assert(a_rkey->current_subkey >= 0 && a_rkey->current_subkey < AMOUNT_SUBKEYS);

	uint16_t ret = a_rkey->keys[a_rkey->current_subkey];
	if (FORWARD == a_rkey->path) {
		++a_rkey->current_subkey;
	} else {
		--a_rkey->current_subkey;
	}
	return ret;
}
