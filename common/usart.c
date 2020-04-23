#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "usart.h"
#include "fifo.h"
#include "led.h"

#define PORT_USART3_TX GPIOB
#define PIN_USART3_TX 10
#define PORT_USART3_RX GPIOB
#define PIN_USART3_RX 12

struct usart_setup_cfg
{
    uint32_t usart_base;
    enum rcc_periph_clken clken;
    enum rcc_periph_rst reset;
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

struct usart_cfg {
    struct fifo * rx_fifo;
    struct fifo * tx_fifo;
    uint32_t usart;
};

struct usart_cfg usart3_cfg;

void itoa(int n, char *buf, size_t len);

#define USART3_FIFO_SZ 1024
struct fifo usart3_fifo_tx;
uint8_t usart3_array_tx[USART3_FIFO_SZ];
struct fifo usart3_fifo_rx;
uint8_t usart3_array_rx[USART3_FIFO_SZ];

static struct usart_cfg * get_usart_cfg(uint32_t usart_base)
{
    if (usart_base == usart3_cfg.usart) {
        return &usart3_cfg;
    }
    return NULL;
}

static void usart_setup(struct usart_setup_cfg *cfg)
{
    uint16_t gpio_tx = (1 << cfg->pin_tx);
    uint16_t gpio_rx = (1 << cfg->pin_rx);
    rcc_periph_reset_pulse(cfg->reset);
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

    usart_enable_rx_interrupt(cfg->usart_base);
    usart_enable(cfg->usart_base);
}

void usart3_setup(void)
{
    struct usart_setup_cfg setup_cfg;

    setup_cfg.usart_base = USART3;
    setup_cfg.clken = RCC_USART3;
    setup_cfg.reset = RST_USART3;
    setup_cfg.port_tx = PORT_USART3_TX;
    setup_cfg.pin_tx = PIN_USART3_TX;
    setup_cfg.alt_func_tx = GPIO_AF7;
    setup_cfg.port_rx = PORT_USART3_RX;
    setup_cfg.pin_rx = PIN_USART3_RX;
    setup_cfg.alt_func_rx = GPIO_AF8;
    setup_cfg.irqn = NVIC_USART3_IRQ;
    setup_cfg.baudrate = 115200;
    setup_cfg.databits = 8;
    setup_cfg.stopbits = USART_STOPBITS_1;
    setup_cfg.mode = USART_MODE_TX_RX;
    setup_cfg.parity = USART_PARITY_NONE;
    setup_cfg.flowcontrol = USART_FLOWCONTROL_NONE;

    usart_setup(&setup_cfg);
    fifo_create(&usart3_fifo_tx, usart3_array_tx, sizeof(usart3_array_tx[0]), USART3_FIFO_SZ);
    fifo_create(&usart3_fifo_rx, usart3_array_rx, sizeof(usart3_array_rx[0]), USART3_FIFO_SZ);

    usart3_cfg.rx_fifo = &usart3_fifo_rx;
    usart3_cfg.tx_fifo = &usart3_fifo_tx;
    usart3_cfg.usart = setup_cfg.usart_base;
}

void usart_send_blocking_str(uint32_t usart_base, const char *str)
{
    for (uint32_t i = 0; i < strlen(str); ++i) {
        usart_send_blocking(usart_base, str[i]);
    }
}

//TODO: return number of sent bytes
void usart_send_blocking_buf(uint32_t usart_base, const char *str, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        usart_send_blocking(usart_base, str[i]);
    }
}

void usart_send_buf(uint32_t usart_base, const char *str, size_t len)
{
    size_t i = 0;
    struct usart_cfg * usart_cfg = get_usart_cfg(usart_base);
    if (usart_cfg == NULL) {
        return;
    }

    while(len--) {
        fifo_push(usart_cfg->tx_fifo, &str[i++]);
    }
    usart_enable_tx_interrupt(usart_cfg->usart);
}

typedef void (*usart_send_fn)(uint32_t, const char *str, size_t len);
static int usart_send_generic_printf(uint32_t usart_base, usart_send_fn send, const char *fmt, va_list arg)
{
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
                send(usart_base, (const char *)'%', 1);
                length++;
                break;

                /* %c: print out a character    */
            case 'c':
                char_temp = va_arg(arg, int);
                send(usart_base, (const char *)&char_temp, 1);
                length++;
                break;

                /* %s: print out a string       */
            case 's':
                string_temp = va_arg(arg, char *);
                send(usart_base, (const char *)string_temp, strlen(string_temp));
                length += strlen(string_temp);
                break;

                /* %d: print out an int         */
            case 'd':
                int_temp = va_arg(arg, int);
                itoa(int_temp, buffer, 10);
                send(usart_base, (const char *)buffer, strlen(buffer));
                length += strlen(buffer);
                break;

                /* %x: print out an int in hex  */
            case 'x':
                int_temp = va_arg(arg, int);
                itoa(int_temp, buffer, 16);
                send(usart_base, (const char *)buffer, strlen(buffer));
                length += strlen(buffer);
                break;

#if 0
            case 'f':
                double_temp = va_arg(arg, double);
                ftoa_fixed(buffer, double_temp);
                send(usart_base, buffer, strlen(buffer));
                length += strlen(buffer);
                break;

            case 'e':
                double_temp = va_arg(arg, double);
                ftoa_sci(buffer, double_temp);
                send(usart_base, buffer, strlen(buffer));
                length += strlen(buffer);
                break;
#endif
            }
        }
        else {
            send(usart_base, (const char *)&ch, 1);
            length++;
        }
    }
    va_end(arg);
    return length;
}

int usart_send_blocking_printf(uint32_t usart_base, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    return usart_send_generic_printf(usart_base, &usart_send_blocking_buf, fmt, arg);
}

int usart_send_printf(uint32_t usart_base, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    return usart_send_generic_printf(usart_base, &usart_send_buf, fmt, arg);
}

static void uartx_isr(struct usart_cfg * usart_cfg)
{
    uint32_t usart_base = usart_cfg->usart;
    struct fifo * tx_fifo = usart_cfg->tx_fifo;
    struct fifo * rx_fifo = usart_cfg->rx_fifo;
    uint8_t data;
    /* Check if we were called because of RXNE. */
    if (((USART_CR1(usart_base) & USART_CR1_RXNEIE) != 0) &&
        ((USART_ISR(usart_base) & USART_FLAG_RXNE) != 0)) {
        /* Retrieve the data from the peripheral, and store it in rx fifo. */
        data = usart_recv(usart_base);
        fifo_push(rx_fifo, &data);
    }

    /* Check if we were called because of TXE. */
    if (((USART_CR1(usart_base) & USART_CR1_TXEIE) != 0) &&
        ((USART_ISR(usart_base) & USART_FLAG_TXE) != 0)) {

        if (!fifo_is_empty(tx_fifo))
        {
            fifo_pop(tx_fifo, &data);
            /* Put data into the transmit register. */
            usart_send(usart_base, data);
        } else {
            /* Disable the TXE interrupt as we don't need it anymore. */
            usart_disable_tx_interrupt(usart_base);
        }
    }
}

void usart3_isr(void)
{
    uartx_isr(&usart3_cfg);
}
