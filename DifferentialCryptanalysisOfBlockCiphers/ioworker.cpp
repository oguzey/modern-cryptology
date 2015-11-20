#include "ioworker.h"

/**
 * @brief get_size_file - Return size of file, in bytes.
 * @param a_filename    - Name of file.
 * @return              - Size of file if file was handled successfully
 *                        -1 - error was occurred.
 */
static int IOWorker::get_size_file(const char *a_filename)
{
	struct stat sb;

	if (!a_filename) {
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
static int IOWorker::read_file(const char *a_filename, uint8_t **a_output)
{

	FILE *file = NULL;
	uint8_t *data = NULL;
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

	if ((file = fopen(a_filename, "r")) == NULL) {
		error("Could not open file '%s'", a_filename);
		return -1;
	}

	data = malloc(sizeof(uint8_t) * size);
	if (data == NULL) {
		error("Cannot allocate memory.");
		fclose(file);
		return -1;
	}
	memset(data, 0, sizeof(uint8_t) * size);
	i = 0;
	shift = 0;
	while ((c = fgetc(file)) != EOF) {
		data[i] = c;
	}
	fclose(file);
	*a_output = data;
	return size;
}

static void IOWorker::write_to_file(uint16_t *a_input, size_t a_size, const char *a_filename)
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

static int IOWorker::convert2unit16_t(const unit8_t *a_input, size_t a_size, uint16_t **a_output)
{
	uint16_t *data = NULL;
	size_t index = 0;
	size_t new_size = a_size / 2;

	if (a_size & 1) {
		++new_size;
	}

	data = new int[sizeof(uint16_t) * new_size];
	if (!data) {
		error("Cannot allocate memory.");
		return -1;
	}
	memset(data, 0, sizeof(uint16_t) * size);

	index = 0;
	for(size_t i = 0; i < a_size; ++i) {

		if (i & 1) {
			(*data) += (a_input[i] << 8);
			++data;
		} else {
			*data = a_input[i];
		}
	}
	*a_output = data;
}
