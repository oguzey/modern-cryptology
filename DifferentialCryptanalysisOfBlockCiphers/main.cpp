#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include "RoundKey.h"
#include "HeysCipher.h"
#include "../CommonLib/ioworker.h"

#define ALLOCATE
#include "../CommonLib/logger.h"



static Action _s_action = UNDEFINED;
static char *_s_input_file = NULL;
static char *_s_output_file = NULL;
static char *_s_key_file = NULL;

static inline void get_args(int argc, char **argv);

int main(int argc, char **argv)
{
	int res = 0;
	std::vector<uint16_t> subkeys;
	std::vector<uint16_t> input;

	get_args(argc, argv);

	if (_s_key_file) {
		res = IOWorker::read_file(_s_key_file, subkeys);
		if (res < 0) {
			logger.critical("Error occurred during reading key-file.");
		} else if (res == AMOUNT_SUBKEYS_IN_BYTES) {
			logger.critical("Wrong length of key.");
		}
	}

	res = IOWorker::read_file(_s_input_file, input);
	if (res < 0) {
		logger.critical("Error occurred during reading input-file.");
	}

	logger.info("input size: {}", input.size());
	for (int i = 0; i < input.size(); ++i) {
		logger.info("{0} {1:x}", i, input[i]);
	}

	if (_s_action == UNDEFINED) {
		logger.critical("Action is wrong.");
	}

	logger.info("subkeys size {}", subkeys.size());
	for (int i = 0; i < subkeys.size(); ++i) {
		logger.info("{0} {1:x}", i, subkeys[i]);
	}

	RoundKey round_key(subkeys, _s_action == ENCRYPT ? FORWARD : BACKWARD);
	HeysCipher heys_cipher(&round_key);

	std::vector<uint16_t> output;
	heys_cipher.run(_s_action, input, output);

	logger.info("output size: {}", output.size());
	for (int i = 0; i < output.size(); ++i) {
		logger.info("{0} {1:x}", i, output[i]);
	}

	IOWorker::write_to_file(output, _s_output_file);

	logger.info("End program.");
	return 0;
}



static inline void get_args(int argc, char **argv)
{
	std::string info("Usage: HeysCipher [-e|-d] -i <input-file> -o <output-file> -k <key-file>\n");

	int i;
	for (i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encrypt") == 0) {
			_s_action = ENCRYPT;
		} else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decrypt") == 0) {
			_s_action = DECRYPT;
		} else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input-file") == 0) {
			if (argc > i + 1) {
				_s_input_file = argv[i + 1];
				i++;
			} else {
				logger.critical("Missing argument for option %s", argv[i]);
			}
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output-file") == 0) {
			if (argc > i + 1) {
				_s_output_file = argv[i + 1];
				i++;
			} else {
				logger.critical("Missing argument for option %s", argv[i]);
			}
		} else if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--key-file") == 0) {
			if (argc > i + 1) {
				_s_key_file = argv[i + 1];
				i++;
			} else {
				logger.critical("Missing argument for option %s", argv[i]);
			}
		} else {
			logger.critical("{}", info);
		}
	}
	if (!_s_input_file || !_s_output_file || _s_action == UNDEFINED) {
		logger.critical("{}", info);
	}
}