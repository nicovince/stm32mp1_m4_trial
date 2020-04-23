#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "usart.h"
#include "fifo.h"

#define PORT_USART3_TX GPIOB
#define PIN_USART3_TX 10
#define PORT_USART3_RX GPIOB
#define PIN_USART3_RX 12

struct usart_setup_cfg
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

void itoa(int n, char *buf, size_t len);

#define USART3_FIFO_SZ 1024
struct fifo usart3_fifo_tx;
uint8_t usart3_array_tx[USART3_FIFO_SZ];
struct fifo usart3_fifo_rx;
uint8_t usart3_array_rx[USART3_FIFO_SZ];

static void usart_setup(struct usart_setup_cfg *cfg)
{
    uint16_t gpio_tx = (1 << cfg->pin_tx);
    uint16_t gpio_rx = (1 << cfg->pin_rx);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(cfg->clken);
    nvic_enable_irq(cfg->irqn);

    gpio_mode_setup(cfg->port_tx, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_tx);
    gpio_mode_setup(cfg->port_rx, GPIO_MODE_AF, GPIO_PUPD_NONE, gpio_rx);
    gpio_set_output_options(cfg->port_rx, GPIO_OTYPE_OD, GPIO_OSPEED_MED, gpio_rx);

    gpio_set_af(cfg->port_tx, cfg->alt_func_tx, gpio_tx);
    gpio_set_af(cfg->port_rx, cfg->alt_func_rx, gpio_rx);

    usart_set_baudrate(cfg->usart_base, cfg->baudrate);
    usart_set_databits(cfg->usart_base, cfg->databits);
    usart_set_stopbits(cfg->usart_base, cfg->stopbits);
    usart_set_mode(cfg->usart_base, cfg->mode);
    usart_set_parity(cfg->usart_base, cfg->parity);
    usart_set_flow_control(cfg->usart_base, cfg->flowcontrol);

    //usart_enable_rx_interrupt(cfg->usart_base);
    usart_enable(cfg->usart_base);
}

void usart3_setup(void)
{
    struct usart_setup_cfg usart3_cfg;

    usart3_cfg.usart_base = USART3;
    usart3_cfg.clken = RCC_USART3;
    usart3_cfg.port_tx = PORT_USART3_TX;
    usart3_cfg.pin_tx = PIN_USART3_TX;
    usart3_cfg.alt_func_tx = GPIO_AF7;
    usart3_cfg.port_rx = PORT_USART3_RX;
    usart3_cfg.pin_rx = PIN_USART3_RX;
    usart3_cfg.alt_func_rx = GPIO_AF8;
    usart3_cfg.irqn = NVIC_USART3_IRQ;
    usart3_cfg.baudrate = 115200;
    usart3_cfg.databits = 8;
    usart3_cfg.stopbits = USART_STOPBITS_1;
    usart3_cfg.mode = USART_MODE_TX_RX;
    usart3_cfg.parity = USART_PARITY_NONE;
    usart3_cfg.flowcontrol = USART_FLOWCONTROL_NONE;

    usart_setup(&usart3_cfg);
    fifo_create(&usart3_fifo_tx, usart3_array_tx, sizeof(usart3_array_tx[0]), USART3_FIFO_SZ);
    fifo_create(&usart3_fifo_rx, usart3_array_rx, sizeof(usart3_array_rx[0]), USART3_FIFO_SZ);
}

void usart_send_blocking_str(uint32_t usart_base, const char *str)
{
    for (uint32_t i = 0; i < strlen(str); ++i) {
        usart_send_blocking(usart_base, str[i]);
    }
}

void usart_send_blocking_buf(uint32_t usart_base, const char *str, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        usart_send_blocking(usart_base, str[i]);
    }
}

int usart_send_blocking_printf(uint32_t usart_base, char const *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    int int_temp;
    char char_temp;
    char *string_temp;

    char ch;
    int length = 0;

    char buffer[512];

    while ( (ch = *fmt++)) {
        if ( '%' == ch ) {
            switch (ch = *fmt++) {
                /* %% - print out a single %    */
            case '%':
                usart_send_blocking_buf(usart_base, (const char *)'%', 1);
                length++;
                break;

                /* %c: print out a character    */
            case 'c':
                char_temp = va_arg(arg, int);
                usart_send_blocking_buf(usart_base, (const char *)&char_temp, 1);
                length++;
                break;

                /* %s: print out a string       */
            case 's':
                string_temp = va_arg(arg, char *);
                usart_send_blocking_buf(usart_base, (const char *)string_temp, strlen(string_temp));
                length += strlen(string_temp);
                break;

                /* %d: print out an int         */
            case 'd':
                int_temp = va_arg(arg, int);
                itoa(int_temp, buffer, 10);
                usart_send_blocking_buf(usart_base, (const char *)buffer, strlen(buffer));
                length += strlen(buffer);
                break;

                /* %x: print out an int in hex  */
            case 'x':
                int_temp = va_arg(arg, int);
                itoa(int_temp, buffer, 16);
                usart_send_blocking_buf(usart_base, (const char *)buffer, strlen(buffer));
                length += strlen(buffer);
                break;

#if 0
            case 'f':
                double_temp = va_arg(arg, double);
                ftoa_fixed(buffer, double_temp);
                usart_send_blocking_buf(usart_base, buffer, strlen(buffer));
                length += strlen(buffer);
                break;

            case 'e':
                double_temp = va_arg(arg, double);
                ftoa_sci(buffer, double_temp);
                usart_send_blocking_buf(usart_base, buffer, strlen(buffer));
                length += strlen(buffer);
                break;
#endif
            }
        }
        else {
            usart_send_blocking_buf(usart_base, (const char *)&ch, 1);
            length++;
        }
    }
    va_end(arg);
    return length;
}
