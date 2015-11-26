#ifndef IOWORKER_H
#define IOWORKER_H

#include <stdint.h>

class IOWorker
{
public:
	static int get_size_file(const char *a_filename);
	static int read_file(const char *a_filename, std::vector<uint8_t> **a_output);
	static int read_file(const char *a_filename, std::vector<uint16_t> **a_output);
	static void write_to_file(std::vector<uint8_t> *a_input, const char *a_filename);
	static size_t convert2unit16_t(const std::vector<uint8_t> *a_in, std::vector<uint16_t> **a_out);
	static int convert2unit8_t(const std::vector<uint16_t> *a_in, std::vector<uint8_t> **a_out);
};
#endif // IOWORKER_H
