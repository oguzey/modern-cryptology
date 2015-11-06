#ifndef ROUNDKEY_H
#define ROUNDKEY_H

#include <stdint.h>

typedef struct round_key RoundKey;

RoundKey* round_key_create(void);

void round_key_destroy(RoundKey *a_rkey);

void round_key_generate(RoundKey *a_rkey);

int16_t round_key_next(RoundKey *a_rkey);

#endif // ROUNDKEY_H
