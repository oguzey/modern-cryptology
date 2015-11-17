#ifndef ROUNDKEY_H
#define ROUNDKEY_H

#include <stdint.h>

#define AMOUNT_SUBKEYS			7
#define AMOUNT_SUBKEYS_IN_BYTES		AMOUNT_SUBKEYS * sizeof(uint16_t)

typedef struct round_key RoundKey;

RoundKey* round_key_create(uint16_t *a_start_value);

void round_key_destroy(RoundKey *a_rkey);

void round_key_generate_new(RoundKey *a_rkey);

void round_key_set_key(RoundKey *a_rkey, uint16_t *a_new_keys);

void round_key_reset_using_to_start(RoundKey *a_rkey);

void round_key_reset_using_to_end(RoundKey *a_rkey);

uint16_t round_key_next_subkey(RoundKey *a_rkey);

#endif // ROUNDKEY_H
