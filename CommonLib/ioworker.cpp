#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include "ioworker.h"
#include "logger.h"

/**
 * @brief get_size_file - Return size of file, in bytes.
 * @param a_filename    - Name of file.
 * @return              - Size of file if file was handled successfully
 *                        -1 - error was occurred.
 */
int IOWorker::get_size_file(const char *a_filename)
{
	struct stat sb;

	if (!a_filename) {
		logger.warn("File not provided.");
		return -1;
	}

	if (stat(a_filename, &sb) == -1) {
		logger.critical("Could not process file '%s'", a_filename);
		return -1;
	}
	logger.info("The file '%s' has size: %d bytes", a_filename,
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
int IOWorker::read_file(const char *a_filename, std::vector<uint8_t> &a_output)
{
	FILE *file = NULL;
	int c = 0;
	int res = 0;
	int size = 0;

	if ((res = get_size_file(a_filename)) < 0) {
		logger.warn("Error was occurred.");
		return -1;
	}
	size = res;

	if ((file = fopen(a_filename, "r")) == NULL) {
		logger.critical("Could not open file '%s'", a_filename);
		return -1;
	}

	while ((c = fgetc(file)) != EOF) {
		a_output.push_back((uint8_t)c);
	}
	fclose(file);
	return size;
}

int IOWorker::read_file(const char *a_filename, std::vector<uint16_t> &a_output)
{
	std::vector<uint8_t> out8;
	int ret = 0;

	ret = IOWorker::read_file(a_filename, out8);
	if (ret < 0) {
		return ret;
	}

	logger.info("read_file out8 size: {}", out8.size());
	for (int i = 0; i < out8.size(); ++i) {
		logger.info("{0} {1:x}", i, out8[i]);
	}

	ret = (int) IOWorker::convert2unit16_t(out8, a_output);
	return ret;
}

void IOWorker::write_to_file(std::vector<uint8_t> &a_input, const char *a_filename)
{
	FILE *file;

	if ((file = fopen(a_filename, "w")) == NULL) {
		logger.critical("Could not open file '%s'", a_filename);
		exit(1);
	}

	for(auto iter = a_input.begin(); iter != a_input.end(); ++iter) {
		fputc(*iter, file);
	}

	fclose(file);
}

void IOWorker::write_to_file(std::vector<uint16_t> &a_input, const char *a_filename)
{
	std::vector<uint8_t> input8;
	IOWorker::convert2unit8_t(a_input, input8);

	logger.info("write_to_file output size: {}", input8.size());
	for (int i = 0; i < a_input.size(); ++i) {
		logger.info("{0} {1:x}", i, a_input[i]);
	}

	IOWorker::write_to_file(input8, a_filename);
}

size_t IOWorker::convert2unit16_t(const std::vector<uint8_t> &a_in, std::vector<uint16_t> &a_out)
{
	size_t size_input = a_in.size();
	size_t size_output = size_input / 2;
	uint16_t value = 0;

	if (size_input & 1) {
		++size_output;
	}

	for(size_t i = 0; i < size_input; ++i) {
		if ((i & 1) == 0) {
			value = a_in.at(i);
		} else {
			value += (a_in.at(i) << 8);
			a_out.push_back(value);
		}
	}
	return size_output;
}


int IOWorker::convert2unit8_t(const std::vector<uint16_t> &a_in, std::vector<uint8_t> &a_out)
{
	int i = 0;
	for (auto it = a_in.begin(); it != a_in.end(); ++it) {
		a_out.push_back((uint8_t)(*it & 0xFF));
		a_out.push_back((uint8_t)(*it >> 8 & 0xFF));
	}
}


