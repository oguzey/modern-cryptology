#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "RoundKey.h"
#include "HeysCipher.h"

static inline int get_size_file(const char *a_filename);
static inline int read_file(const char *a_filename, uint16_t **a_output);

int main(void)
{
	int res = 0;
	size_t size = 0;
	RoundKey *round_key = NULL;
	HeysCipher *heys_cipher = NULL;

	uint16_t *input = NULL;
	uint16_t *output = NULL;

	res = read_file("file", &input);
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
