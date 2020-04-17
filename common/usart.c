#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include <string.h>

#include "usart.h"

void usart_setup(struct usart_cfg *cfg)
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

void usart_send_blocking_str(uint32_t usart_base, const char *str)
{
    for (uint32_t i = 0; i < strlen(str); ++i) {
        usart_send_blocking(usart_base, str[i]);
    }
}
