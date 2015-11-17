#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include <stdlib.h>

#define info(format, ...) printf("[%s] " format "\n", __func__,  ##__VA_ARGS__)

#define info_msg(message) printf("[%s] %s\n", __func__, message)

#define info_hex_uint16(value) printf("[%s] %s = %04x\n", __func__, #value, value)

#define info_bin_uint16(value)					\
	do {							\
		printf("[%s] %s = ", __func__, #value);		\
		int printed_symbols = 0;			\
		int i = 1 << 15;				\
		for (;i > 0; i >>= 1) {				\
			printf( (value & i) ? "1" : "0");	\
			if (++printed_symbols % 4 == 0) {	\
				printf(" ");			\
			}					\
		}						\
	} while (0)


#define fatal(format, ...)							\
	fprintf(stderr, "[%s] " format "\n", __func__,  ##__VA_ARGS__);		\
	exit(-1)

#endif // LOG_H
