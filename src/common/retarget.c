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
    USART_TypeDef* interface = USB_UART;

    if(RADIO_FD == fd) {
        interface = USART1;
    }

	for (fd = 0; (size_t) fd < count; fd++) {
		if (_putc(interface, BLOCK, *((char *) buf_ptr++)))
			return fd;
	}
	return count;
}

int _read(int fd, const void *buf, size_t count) {
    const char * buf_ptr = buf;
    USART_TypeDef* interface = USB_UART;

    if(RADIO_FD == fd) {
        interface = USART1;
    }

	for (fd = 0; (size_t) fd < count; fd++) {
		if (_getc(interface, BLOCK, (char *) buf_ptr++))
			return fd;
	}
	return count;
}
