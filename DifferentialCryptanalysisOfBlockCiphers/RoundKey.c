#include "RoundKey.h"
#include <stdint.h>

struct round_key {
    int16_t keys[7];
};

RoundKey* round_key_create(void)
{
    //TODO: implement me
    return NULL;
}

void round_key_destroy(RoundKey *a_rkey)
{
    //TODO: implement me
}

void round_key_generate(RoundKey *a_rkey)
{
    //TODO: implement me
}

int16_t round_key_next(RoundKey *a_rkey)
{
    //TODO: implement me
    return -1;
}
