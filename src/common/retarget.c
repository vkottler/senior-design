/*
 * stdio redirection
 */

#include <stdio.h>
#include "pcbuffer.h"
#include "usart.h"
#include "board.h"

#define BLOCK	true

int _write(int fd, const void *buf, size_t count) {
    const char * buf_ptr = buf;
	for (fd = 0; (size_t) fd < count; fd++) {
		if (_putc(USART1, BLOCK, *((char *) buf_ptr++)))
			return fd;
	}
	return count;
}

int _read(int fd, const void *buf, size_t count) {
    const char * buf_ptr = buf;
	for (fd = 0; (size_t) fd < count; fd++) {
		if (_getc(USART1, BLOCK, (char *) buf_ptr++))
			return fd;
	}
	return count;
}
