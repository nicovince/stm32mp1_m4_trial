#ifndef __USART_H__
#define __USART_H__

#include <libopencm3/stm32/rcc.h>

#include <string.h>

void usart_send_blocking_str(uint32_t usart_base, const char *str);
void usart_send_blocking_buf(uint32_t usart_base, const char *str, size_t len);
int usart_send_blocking_printf(uint32_t usart_base, char const *fmt, ...);

void usart3_setup(void);
#endif /* __USART_H__ */

