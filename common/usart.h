#ifndef __USART_H__
#define __USART_H__

#include <libopencm3/stm32/rcc.h>

#include <string.h>
struct usart_cfg
{
    uint32_t usart_base;
    enum rcc_periph_clken clken;
    uint32_t port_tx;
    uint8_t pin_tx;
    uint8_t alt_func_tx;
    uint32_t port_rx;
    uint8_t pin_rx;
    uint8_t alt_func_rx;
    uint8_t irqn;
    uint32_t baudrate;
    uint8_t databits;
    uint32_t stopbits;
    uint32_t mode;
    uint32_t parity;
    uint32_t flowcontrol;
};

void usart_setup(struct usart_cfg *cfg);
void usart_send_blocking_str(uint32_t usart_base, const char *str);
void usart_send_blocking_buf(uint32_t usart_base, const char *str, size_t len);
int usart_send_blocking_printf(uint32_t usart_base, char const *fmt, ...);

#endif /* __USART_H__ */

