#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include "ioworker.h"
#include "Log.h"

template<std::size_t SIZE>

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
	return (int)sb.st_size;
}

/**
 * @brief read_file     - Read file and write data to a_output.
 * @param a_filename    - Name of file for read.
 * @param a_output      - Container for saving read data.
 * @return              - Size of output container or
 *                        -1 if error was occurred.
 */
static int IOWorker::read_file(const char *a_filename, std::vector<uint8_t> **a_output)
{
	FILE *file = NULL;
	int c = 0;
	int res = 0;
	int size = 0;
	std::vector<uint8_t> *data = nullptr;

	if ((res = get_size_file(a_filename)) < 0) {
		warn("Error was occurred.");
		return -1;
	}
	size = res;

	if ((file = fopen(a_filename, "r")) == NULL) {
		error("Could not open file '%s'", a_filename);
		return -1;
	}

	data = new std::vector<uint8_t>(size);
	while ((c = fgetc(file)) != EOF) {
		data->push_back(c);
	}
	fclose(file);
	*a_output = data;
	return size;
}

static void IOWorker::write_to_file(std::vector<uint8_t> *a_input, const char *a_filename)
{
	FILE *file;

	if ((file = fopen(a_filename, "w")) == NULL) {
		error("Could not open file '%s'", a_filename);
		exit(1);
	}

	for(auto iter = a_input->begin(); iter != a_input->end(); ++iter) {
		fputc(*iter, file);
	}

	fclose(file);
}

static int IOWorker::convert2unit16_t(const std::vector<uint8_t> *a_in, std::vector<uint16_t> **a_out)
{
	std::vector<uint16_t> *data = nullptr;
	size_t size_input = a_in->size();
	size_t size_output = size_input / 2;

	if (size_input & 1) {
		++size_output;
	}

	data = new std::vector<uint16_t>(size_output);

	auto data_iter = data->begin();
	for(size_t i = 0; i < size_input; ++i) {
		if ((i & 1) == 0) {
			*data_iter = a_in->at(i);
		} else {
			(*data_iter) += (a_in->at(i) << 8);
			++data_iter;
		}
	}
	*a_out = data;
	return 0;
}


static int IOWorker::convert2unit8_t(const std::vector<uint16_t> *a_in, std::vector<uint8_t> **a_out)
{
	size_t size_output = a_in->size() * 2;
	*a_out = new std::vector<uint8_t>(size_output);

	auto out_iter = (*a_out)->begin();

	for (auto it = a_in->begin(); it != a_in->end(); ++it) {
		*out_iter = (uint8_t)(*it & 0xFF);
		++out_iter;
		*out_iter = (uint8_t)(*it >> 8 & 0xFF);
		++out_iter;
	}
}