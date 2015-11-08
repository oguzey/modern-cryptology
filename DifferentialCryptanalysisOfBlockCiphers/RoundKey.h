#ifndef ROUNDKEY_H
#define ROUNDKEY_H

#include <stdint.h>

typedef struct round_key RoundKey;

RoundKey* round_key_create(void);

void round_key_destroy(RoundKey *a_rkey);

void round_key_generate_new(RoundKey *a_rkey);

void round_key_set_key(RoundKey *a_rkey, uint16_t *a_new_keys);

void round_key_reset_using(RoundKey *a_rkey);

uint16_t round_key_next(RoundKey *a_rkey);

#endif // ROUNDKEY_H
