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
#ifndef NUCLEO
    return USART1;
#else
    return USART2;
#endif
}

int write_frame(frame_type_t frame_type, const char *buf, size_t count)
{
    size_t curr;

    /* if we can't write this frame in one shot, don't try */
    if (pc_buffer_space(tx_buf[0]) < count + 4)
        return 0;

    /* frame size is one byte, can't send anything larger  */
    if (count > 255) return 0;

    /* write SOF, type, size, body, then EOF */
    _putc(USART1, BLOCK, TELEM_SOF);
    _putc(USART1, BLOCK, (char) frame_type);
    _putc(USART1, BLOCK, (char) (count & 0xff));
	for (curr = 0; curr < count; curr++)
        _putc(USART1, BLOCK, *(buf++));
    _putc(USART1, BLOCK, TELEM_EOF);

	return count;
}

int _write(int fd, const void *buf, size_t count)
{
#ifndef NUCLEO
    return write_frame(TELEM_FRAME_CONSOLE, buf, count);
#else
    size_t curr;
    const char *buf_ptr = buf;
    USART_TypeDef *interface = fd_to_usart(fd);
	for (curr = 0; curr < count; curr++) {
		if (_putc(interface, BLOCK, *(buf_ptr)++))
			return curr;
    }
    return count;
#endif
}

int _read(int fd, void *buf, size_t count)
{
    size_t curr;
    char *buf_ptr = buf;
    USART_TypeDef *interface = fd_to_usart(fd);
	for (curr = 0; curr < count; curr++) {
		if (_getc(interface, BLOCK, buf_ptr++))
			return curr;
	}
	return count;
}
