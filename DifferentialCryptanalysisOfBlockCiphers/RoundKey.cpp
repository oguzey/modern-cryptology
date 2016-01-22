#include "RoundKey.h"
#include "../CommonLib/logger.h"

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
