#include "RoundKey.h"
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include "../CommonLib/logger.h"

//mc::Logger& logger = mc::Logger::get_logger();

//enum direction {
//	UNKNOWN,
//	FORWARD,
//	BACKWARD
//};
//
//struct round_key {
//	uint16_t keys[AMOUNT_SUBKEYS];
//	int current_subkey;
//	enum direction path;
//};
//
//
//RoundKey* round_key_create(uint16_t *a_start_value)
//{
//	RoundKey *rkey = malloc(sizeof(RoundKey));
//	rkey->current_subkey = 0;
//	if (a_start_value) {
//		round_key_set_key(rkey, a_start_value);
//	} else {
//		round_key_generate_new(rkey);
//	}
//	return rkey;
//}
//
//void round_key_destroy(RoundKey *a_rkey)
//{
//	if (a_rkey) {
//		free(a_rkey);
//	}
//
//}
//
//void round_key_generate_new(RoundKey *a_rkey)
//{
//	assert(a_rkey);
//
//	int i = 0;
//
//	a_rkey->path = UNKNOWN;
//
//	srand(time(NULL));
//	for(i = 0; i < AMOUNT_SUBKEYS; ++i) {
//		a_rkey->keys[i] = UINT16_C(rand()) % UINT_LEAST16_MAX;
//	}
//}
//
//void round_key_set_key(RoundKey *a_rkey, uint16_t *a_new_keys)
//{
//	assert(a_rkey);
//	assert(a_new_keys);
//
//	int i = 0;
//
//	a_rkey->path = UNKNOWN;
//
//	for(i = 0; i < AMOUNT_SUBKEYS; ++i) {
//		a_rkey->keys[i] = a_new_keys[i];
//	}
//
//}
//
//void round_key_reset_using_to_start(RoundKey *a_rkey)
//{
//	assert(a_rkey);
//	a_rkey->path = FORWARD;
//	a_rkey->current_subkey = 0;
//}
//
//void round_key_reset_using_to_end(RoundKey *a_rkey)
//{
//	assert(a_rkey);
//	a_rkey->path = BACKWARD;
//	a_rkey->current_subkey = AMOUNT_SUBKEYS - 1;
//}
//
//uint16_t round_key_next_subkey(RoundKey *a_rkey)
//{
//	assert(a_rkey);
//	assert(UNKNOWN != a_rkey->path);
//	assert(a_rkey->current_subkey >= 0 && a_rkey->current_subkey < AMOUNT_SUBKEYS);
//
//	uint16_t ret = a_rkey->keys[a_rkey->current_subkey];
//	if (FORWARD == a_rkey->path) {
//		++a_rkey->current_subkey;
//	} else {
//		--a_rkey->current_subkey;
//	}
//	return ret;
//}

RoundKey::RoundKey() : RoundKey(FORWARD) {
	logger.debug("RoundKey default constructor called");
}

RoundKey::RoundKey(Direction a_path) : path(a_path) {
	logger.debug("RoundKey constructor with direction was called");
	reset_position();
	generate_new();
}

RoundKey::RoundKey(const std::vector<uint16_t> &a_keys, Direction a_path): path(a_path)
{
	for (int i = 0; i < amount_subkeys; ++i) {
		subkeys[i] = a_keys[i];
	}
	logger.debug("RoundKey constructor with params called with path '{}'", a_path);
	reset_position();
}

void RoundKey::generate_new() {
	srand((unsigned int) time(NULL));
	for (auto index = 0; index < subkeys.size(); ++index) {
		subkeys[index] = (uint16_t)((rand()) % UINT_LEAST16_MAX);
	}
}

void RoundKey::set_key(const std::array<uint16_t, amount_subkeys> &a_subkeys) {
	// CHECK ME: I am not sure that is correct
	subkeys = a_subkeys;
}

void RoundKey::reset_position() {
	switch (path) {
		case FORWARD:
			pos_current_subkey = 0;
			break;
		case BACKWARD:
			pos_current_subkey = amount_subkeys - 1;
			break;
		default:
			logger.critical("Wrong value of direction was provided. Value: '{}'", path);
			break;
	}
}

uint16_t RoundKey::next_subkey() {
	uint16_t key = subkeys.at(pos_current_subkey);
	switch (path) {
		case FORWARD:
			++pos_current_subkey;
			break;
		case BACKWARD:
			--pos_current_subkey;
			break;
		default:
			logger.critical("Wrong value of direction was provided. Value: '{}'", path);
			break;
	}
	return key;
}
