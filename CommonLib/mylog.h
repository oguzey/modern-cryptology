#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#define fatal(format, ...)							\
	fprintf(stderr, "[%s] " format "\n", __func__,  ##__VA_ARGS__);		\
	exit(-1)

#ifdef DEBUG

#define log(level, format, ...) printf("[%s] " level ": " format "\n", __func__,  ##__VA_ARGS__)

#else

#define log(level, format, ...) printf(level ": " format "\n", ##__VA_ARGS__)

#endif // DEBUG

static void inline log_bin_uint(uint64_t value, size_t amount_bits,
				const char *name_var, const char *level, ...)
{
	char buffer[90];
	memset(&buffer, '\0', 90);

	int index = 0;
	int printed_symbols = 0;
	int i = 0;

	for (i = 1 << (amount_bits - 1); i > 0; i >>= 1) {

		buffer[index++] = (value & i) ? '1' : '0';

		if (++printed_symbols % 4 == 0) {
			buffer[index++] = ' ';
		}
	}

#ifdef DEBUG
	va_list args;
	va_start(args, level);
	char *func = va_arg(args, char*);
	printf("[%s] %s: %s = %s\n", func, level, name_var, buffer);
#else
	printf("%s: %s = %s\n", level, name_var, buffer);
#endif // DEBUG
}


#ifdef DEBUG

#define info(format, ...) log("info", format, ##__VA_ARGS__)
#define info_hex_uint16(value) log("info", "%s = %04x\n", #value, value)
#define info_bin_uint16(value)	log_bin_uint(value, 16, #value, "info", __func__)

#define debug(format, ...) log("debug", format, ##__VA_ARGS__)
#define debug_hex_uint16(value) log("debug", "%s = %04x\n", #value, value)
#define debug_bin_uint16(value)	log_bin_uint(value, 16, #value, "debug", __func__)

#define warn(format, ...) log("warn", format, ##__VA_ARGS__)

#define error(format, ...)	\
		log("error", format, ##__VA_ARGS__);		\
		log("error", "Error was: %s", strerror(errno))

#else

#define info(format, ...) log("info", format, ##__VA_ARGS__)
#define info_hex_uint16(value) log("info", "%s = %04x\n", #value, value)
#define info_bin_uint16(value)	log_bin_uint(value, 16, #value, "info")

#define debug(format, ...)
#define debug_hex_uint16(value)
#define debug_bin_uint16(value)

#define warn(format, ...) log("warn", format, ##__VA_ARGS__)

#define error(format, ...)	\
		log("error", format, ##__VA_ARGS__);		\
		log("error", "Error was: %s", strerror(errno))

#endif // DEBUG

#endif // LOG_H
