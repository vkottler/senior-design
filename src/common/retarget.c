/*
 * stdio redirection
 */

#include <stdio.h>
#include "pcbuffer.h"
#include "usart.h"
#include "board.h"

#include "frame.h"

#define BLOCK	true

USART_TypeDef *fd_to_usart(int fd)
{
    return USB_UART;
}

int _write(int fd, const void *buf, size_t count) {
    size_t curr;
    const char *buf_ptr = buf;
    USART_TypeDef *interface = fd_to_usart(fd);

    if (count > 255)
        return -1;

    /* write SOF */
    if (_putc(interface, BLOCK, TELEM_SOF))
        return -1;

    /* write type */
    if (_putc(interface, BLOCK, (char) TELEM_FRAME_CONSOLE))
        return -1;

    /* write size */
    if (_putc(interface, BLOCK, (char) (count & 0xff)))
        return -1;

    /* write body */
	for (curr = 0; curr < count; curr++) {
		if (_putc(interface, BLOCK, *((char *) buf_ptr++)))
			return -1;
	}

    /* write EOF */
    if (_putc(interface, BLOCK, TELEM_EOF))
        return -1;

	return count;
}

int _read(int fd, const void *buf, size_t count) {
    size_t curr;
    const char *buf_ptr = buf;
    USART_TypeDef *interface = fd_to_usart(fd);
	for (curr = 0; curr < count; curr++) {
		if (_getc(interface, BLOCK, (char *) buf_ptr++))
			return curr;
	}
	return count;
}
