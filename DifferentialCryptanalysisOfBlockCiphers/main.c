#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "RoundKey.h"
#include "HeysCipher.h"

#define panic(msg, ...) \
    printf(msg, ##__VA_ARGS__); \
    exit(1);

typedef enum {
	ENCRYPT,
	DECRYPT
} Action;

static Action _s_action = -1;
static char *_s_input_file = NULL;
static char *_s_output_file = NULL;
static char *_s_key_file = NULL;

static inline int get_size_file(const char *a_filename);
static inline int read_file(const char *a_filename, uint16_t **a_output);
static inline void get_args(int argc, char **argv);

int main(int argc, char **argv)
{
	int res = 0;
	size_t size = 0;
	RoundKey *round_key = NULL;
	HeysCipher *heys_cipher = NULL;

	uint16_t *input = NULL;
	uint16_t *output = NULL;

	get_args(argc, argv);

	res = read_file(_s_key_file, &input);
	if (res < 0) {
		fprintf(stderr, "Unexpected termination of program.");
		return -1;
	} else if (res == AMOUNT_SUBKEYS_IN_BYTES) {
		return -1;
	}
	size = res;

	res = read_file(_s_input_file, &input);
	if (res < 0) {
		fprintf(stderr, "Unexpected termination of program.");
		return -1;
	}
	size = res;

	round_key = round_key_create();
	heys_cipher = heys_cipher_create(round_key);

	heys_cipher_encrypt(heys_cipher, input, size, &output);

	heys_cipher_destroy(heys_cipher);
	round_key_destroy(round_key);

	printf("Hello World!\n");
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
        printf("File not provided\n");
        return -1;
    }

    if (stat(a_filename, &sb) == -1) {
        printf("Could not process file '%s'\n", a_filename);
        perror("Error was");
        return -1;
    }
    printf("The file '%s' has size: %d bytes\n", a_filename, (int) sb.st_size);
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
    int c, i , shift, res;
	size_t size = 0;

    if ((res = get_size_file(a_filename)) < 0) {
        printf("Error was occurred.\n");
        return -1;
    }
    size = res;

    /* conver to size in bytes to size for array of uint16_t*/
    if (size & 1) {
        size++;
    }
    size /= 2;

	if ((file = fopen(a_filename, "r")) == NULL) {
		printf("Could not open file '%s'\n", a_filename);
		perror("Error was");
		return -1;
	}

	data = malloc(sizeof(uint16_t) * size);
	if (data == NULL) {
		perror("Error was");
		fclose(file);
		return -1;
	}
	memset(data, 0, sizeof(uint16_t) * size);
	i = 0;
	shift = 0;
	while ((c = fgetc(file)) != EOF) {

		data[i] = c << shift * 8;
		shift = ++shift % 2;
		if (!shift) {
			i++;
		}
	}
	fclose(file);
	*a_output = data;
	return size;
}

static inline void get_args(int argc, char **argv)
{
    char *info;
    info = "Usage: HeysCipher [-e|-d] "
			"-i <input-file> -o <output-file> -k <key-file>\n";

    int i;
    for (i = 1; i < argc; ++i) {
	if (strcmp(argv[i],"-e")==0 || strcmp(argv[i],"--encrypt")==0) {
		_s_action = ENCRYPT;
	} else if (strcmp(argv[i],"-d")==0 || strcmp(argv[i],"--decrypt")==0) {
		_s_action = DECRYPT;
	} else if (strcmp(argv[i],"-i")==0 || strcmp(argv[i],"--input-file")==0) {
	    if (argc > i + 1) {
		_s_input_file = argv[i + 1];
		i++;
	    } else {
		panic("Missing argument for option %s\n", argv[i]);
	    }
	} else if (strcmp(argv[i],"-o")==0 || strcmp(argv[i],"--output-file")==0) {
	    if (argc > i + 1) {
		_s_output_file = argv[i + 1];
		i++;
	    } else {
		panic("Missing argument for option %s\n", argv[i]);
	    }
	} else if (strcmp(argv[i],"-k")==0 || strcmp(argv[i],"--key-file")==0) {
	    if (argc > i + 1) {
		_s_key_file = argv[i + 1];
		i++;
	    } else {
		panic("Missing argument for option %s\n", argv[i]);
	    }
	} else {
	    panic("%s\n", info);
	}
    }
    if (!_s_input_file || !_s_output_file || _s_action == -1) {
	panic("%s\n", info);
    }
}
