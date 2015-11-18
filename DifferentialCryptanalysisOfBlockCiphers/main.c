#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "RoundKey.h"
#include "HeysCipher.h"
#include "Log.h"


static Action _s_action = -1;
static char *_s_input_file = NULL;
static char *_s_output_file = NULL;
static char *_s_key_file = NULL;

static inline void write_to_file(uint16_t *a_input, size_t a_size, const char *a_filename);
static inline int get_size_file(const char *a_filename);
static inline int read_file(const char *a_filename, uint16_t **a_output);
static inline void get_args(int argc, char **argv);

int main(int argc, char **argv)
{
	int res = 0;
	size_t size = 0;
	RoundKey *round_key = NULL;
	HeysCipher *heys_cipher = NULL;

	uint16_t *subkeys = NULL;
	uint16_t *input = NULL;
	uint16_t *output = NULL;

	get_args(argc, argv);

	if (_s_key_file) {
		res = read_file(_s_key_file, &subkeys);
		if (res < 0) {
			fatal("Error occurred during reading key-file.");
		} else if (res == AMOUNT_SUBKEYS_IN_BYTES) {
			fatal("Wrong length of key.");
		}
	}

	res = read_file(_s_input_file, &input);
	if (res < 0) {
		fatal("Error occurred during reading input-file.");
	}
	size = res;

	round_key = round_key_create(subkeys);
	heys_cipher = heys_cipher_create(round_key);

	heys_cipher_run(heys_cipher, _s_action, input, size, &output);

	write_to_file(output, size, _s_output_file);

	heys_cipher_destroy(heys_cipher);
	round_key_destroy(round_key);

	free(input);
	free(output);
	free(subkeys);


	info("End program.");
	return 0;
}

/**
 * @brief get_size_file - Return size of file, in bytes.
 * @param a_filename    - Name of file.
 * @return              - Size of file if file was handled successfully
 *                        -1 - error was occurred.
 */
static inline int get_size_file(const char *a_filename)
{
	struct stat sb;

	if (a_filename == NULL) {
		warn("File not provided.");
		return -1;
	}

	if (stat(a_filename, &sb) == -1) {
		error("Could not process file '%s'", a_filename);
		return -1;
	}
	info("The file '%s' has size: %d bytes", a_filename,
	                (int) sb.st_size);
	return sb.st_size;
}

/**
 * @brief read_file     - Read file and write data to a_output.
 * @param a_filename    - Name of file for read.
 * @param a_output      - Container for saving read data.
 * @return              - Size of output container or
 *                        -1 if error was occurred.
 */
static inline int read_file(const char *a_filename, uint16_t **a_output)
{

	FILE *file = NULL;
	uint16_t *data = NULL;
	int c = 0;
	int i = 0;
	int shift = 0;
	int res = 0;
	size_t size = 0;

	if ((res = get_size_file(a_filename)) < 0) {
		warn("Error was occurred.");
		return -1;
	}
	size = res;

	/* convert to size in bytes to size for array of uint16_t*/
	if (size & 1) {
		size++;
	}
	size /= 2;

	if ((file = fopen(a_filename, "r")) == NULL) {
		error("Could not open file '%s'", a_filename);
		return -1;
	}

	data = malloc(sizeof(uint16_t) * size);
	if (data == NULL) {
		error("Cannot allocate memory.");
		fclose(file);
		return -1;
	}
	memset(data, 0, sizeof(uint16_t) * size);
	i = 0;
	shift = 0;
	while ((c = fgetc(file)) != EOF) {

		data[i] = c << shift * 8;
		shift = (shift + 1) % 2;
		if (!shift) {
			i++;
		}
	}
	fclose(file);
	*a_output = data;
	return size;
}

static inline void write_to_file(uint16_t *a_input, size_t a_size, const char *a_filename)
{
	FILE *file;
	int i = 0;

	if ((file = fopen(a_filename, "w")) == NULL) {
		error("Could not open file '%s'", a_filename);
		exit(1);
	}

	for (i = 0; i < (int) a_size; ++i) {
		fputc(a_input[i] & 0xFF, file);
		fputc((a_input[i] >> 8) & 0xFF, file);
	}
	fclose(file);
}

static inline void get_args(int argc, char **argv)
{
	char *info;
	info = "Usage: HeysCipher [-e|-d] "
			"-i <input-file> -o <output-file> -k <key-file>\n";

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
				fatal("Missing argument for option %s", argv[i]);
			}
		} else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output-file") == 0) {
			if (argc > i + 1) {
				_s_output_file = argv[i + 1];
				i++;
			} else {
				fatal("Missing argument for option %s", argv[i]);
			}
		} else if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--key-file") == 0) {
			if (argc > i + 1) {
				_s_key_file = argv[i + 1];
				i++;
			} else {
				fatal("Missing argument for option %s", argv[i]);
			}
		} else {
			fatal("%s", info);
		}
	}
	if (!_s_input_file || !_s_output_file || _s_action == -1) {
		fatal("%s", info);
	}
}
