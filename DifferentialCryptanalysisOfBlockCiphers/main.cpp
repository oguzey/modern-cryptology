#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include "RoundKey.h"
#include "HeysCipher.h"
#include "../CommonLib/ioworker.h"
#include "../CommonLib/logger.h"

mclogger::Logger logger = mclogger::Logger::get_logger();

static Action _s_action = UNDEFINED;
static char *_s_input_file = NULL;
static char *_s_output_file = NULL;
static char *_s_key_file = NULL;

static inline void get_args(int argc, char **argv);

int main(int argc, char **argv)
{
	int res = 0;
	size_t size = 0;
	RoundKey *round_key = NULL;
	HeysCipher *heys_cipher = NULL;

	std::vector<uint8_t> *tmp_data = nullptr;
	std::vector<uint16_t> *subkeys = nullptr;
	std::vector<uint16_t> *input = nullptr;
	std::vector<uint16_t> *output = nullptr;

	get_args(argc, argv);

	if (_s_key_file) {
		res = IOWorker::read_file(_s_key_file, &subkeys);

		if (res < 0) {
			logger.critical("Error occurred during reading key-file.");
		} else if (res == AMOUNT_SUBKEYS_IN_BYTES) {
			logger.critical("Wrong length of key.");
		}
	}

	res = IOWorker::read_file(_s_input_file, &input);
	if (res < 0) {
		logger.critical("Error occurred during reading input-file.");
	}

	round_key = round_key_create(subkeys);
	heys_cipher = heys_cipher_create(round_key);

	heys_cipher_run(heys_cipher, _s_action, input, size, &output);

	write_to_file(output, size, _s_output_file);

	heys_cipher_destroy(heys_cipher);
	round_key_destroy(round_key);

	free(input);
	free(output);
	free(subkeys);


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