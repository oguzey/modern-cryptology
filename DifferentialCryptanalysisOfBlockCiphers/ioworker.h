#ifndef IOWORKER_H
#define IOWORKER_H

class IOWorker
{
public:
	static int get_size_file(const char *a_filename);
	static int read_file(const char *a_filename, uint16_t **a_output);
	static void write_to_file(uint16_t *a_input, size_t a_size, const char *a_filename);
	static int convert2unit16_t(const unit8_t *a_input, uint16_t **a_output);
};
#endif // IOWORKER_H
