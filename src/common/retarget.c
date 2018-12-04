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
    return USART1;
}

int write_frame(frame_type_t frame_type, const char *buf, size_t count)
{
    size_t curr;

    if (count > 255)
        return -1;

    /* write SOF */
    if (_putc(USART1, BLOCK, TELEM_SOF))
        return -1;

    /* write type */
    if (_putc(USART1, BLOCK, (char) frame_type))
        return -1;

    /* write size */
    if (_putc(USART1, BLOCK, (char) (count & 0xff)))
        return -1;

    /* write body */
	for (curr = 0; curr < count; curr++) {
		if (_putc(USART1, BLOCK, *(buf++)))
			return -1;
	}

    /* write EOF */
    if (_putc(USART1, BLOCK, TELEM_EOF))
        return -1;

	return count;
}

int _write(int fd, const void *buf, size_t count)
{
    return write_frame(TELEM_FRAME_CONSOLE, buf, count);
}

int _read(int fd, const void *buf, size_t count)
{
    size_t curr;
    const char *buf_ptr = buf;
    USART_TypeDef *interface = fd_to_usart(fd);
	for (curr = 0; curr < count; curr++) {
		if (_getc(interface, BLOCK, (char *) buf_ptr++))
			return curr;
	}
	return count;
}
