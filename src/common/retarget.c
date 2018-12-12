/*
 * stdio redirection
 */

#include <stdio.h>
#include "pcbuffer.h"
#include "usart.h"
#include "board.h"
#include "gpio.h"
#include "gyro.h"

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

void update_radio_state(void)
{
    bool re_enable = false;

    if (radio_buffer_full && gpio_readPin(GPIOB, 2))
    {
        radio_buffer_full = false;
        re_enable = true;
    }
    if (!radio_transmit_state && ticks > radio_resume) 
    {
        radio_transmit_state = true;
        re_enable = true;
    }

    if (re_enable) USART1->CR1 |= USART_CR1_TXEIE;
}

void console_wait_free(size_t count)
{
    bool all_ready = false;
    bool enough_space = false;

    while (!all_ready)
    {
        run_critical();
        enough_space = (pc_buffer_space(tx_buf[0]) >= count + 4);
        if (radio_transmit_state && !radio_buffer_full && enough_space)
            all_ready = true;
    }
}

int write_frame(frame_type_t frame_type, const char *buf, size_t count)
{
    size_t curr;

    /* frame size is one byte, can't send anything larger  */
    if (count > 255) return 0;

    /* always make sure console frames get sent */
    if (frame_type == TELEM_FRAME_CONSOLE)
        console_wait_free(count);

    /* if we can't write this frame in one shot, don't try */
    if (pc_buffer_space(tx_buf[0]) < count + 4)
        return 0;

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
