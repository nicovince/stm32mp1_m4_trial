#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>

#include "clock.h"
#include "usart.h"
#include "spi.h"

#include "api.h"
#include "api-asm.h"

#define RCC_LED7  RCC_GPIOH
#define PORT_LED7 GPIOH
#define PIN_LED7  7
#define GPIO_LED7 (1 << PIN_LED7)

#define PORT_USART3_TX GPIOB
#define PIN_USART3_TX 10
#define PORT_USART3_RX GPIOB
#define PIN_USART3_RX 12

static void blink(uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i)
    {
        gpio_set(PORT_LED7, GPIO_LED7);
        msleep(200);
        gpio_clear(PORT_LED7, GPIO_LED7);
        msleep(200);
    }
}

int main(void) {
    uint32_t last_ts = 0;
    struct usart_cfg usart3_cfg;
    uint32_t spi_data;
    uint32_t cnt = 0;
    uint32_t spi_sr;

    usart3_cfg.usart_base = USART3;
    usart3_cfg.clken = RCC_USART3;
    usart3_cfg.port_tx = PORT_USART3_TX;
    usart3_cfg.pin_tx = PIN_USART3_TX;
    usart3_cfg.alt_func_tx = GPIO_AF7;
    usart3_cfg.port_rx = PORT_USART3_RX;
    usart3_cfg.pin_rx = PIN_USART3_RX;
    usart3_cfg.alt_func_rx = GPIO_AF8;
    usart3_cfg.irqn = NVIC_USART3_IRQ;
    usart3_cfg.baudrate = 9600;
    usart3_cfg.databits = 8;
    usart3_cfg.stopbits = USART_STOPBITS_1;
    usart3_cfg.mode = USART_MODE_TX_RX;
    usart3_cfg.parity = USART_PARITY_NONE;
    usart3_cfg.flowcontrol = USART_FLOWCONTROL_NONE;
    usart_setup(&usart3_cfg);


    systick_setup();
    rcc_periph_clock_enable(RCC_LED7);
    gpio_mode_setup(PORT_LED7, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LED7);
    gpio_set_output_options(PORT_LED7, GPIO_OTYPE_PP,
                            GPIO_OSPEED_25MHZ,  GPIO_LED7);

    spi4_setup();
    usart_send_blocking_str(USART3, "Hello World\r\n");
    usart_send_blocking_printf(USART3, "CR1:%x\r\n", SPI2S_CR1(SPI4));
    usart_send_blocking_printf(USART3, "CFG1:%x\r\n", SPI_CFG1(SPI4));
    usart_send_blocking_printf(USART3, "CFG2:%x\r\n", SPI_CFG2(SPI4));
    usart_send_blocking_printf(USART3, "SPI2S_SR: %x\r\n", SPI2S_SR(SPI4));

    while(1) {
        if (last_ts + 5000 < mtime()) {
            usart_send_blocking_printf(USART3, "----- [%d] -----\r\n", mtime());
            gpio_toggle(PORT_LED7, GPIO_LED7);
            spi_write8(SPI4, cnt++);
            spi_write8(SPI4, cnt++);
            spi_write8(SPI4, cnt++);
            usart_send_blocking_printf(USART3, "spidata = %d\r\n", mtime(), spi_data);
            usart_send_blocking_printf(USART3, "CR1:%x\r\n", SPI2S_CR1(SPI4));
            usart_send_blocking_printf(USART3, "CR2:%x\r\n", SPI_CR2(SPI4));
            usart_send_blocking_printf(USART3, "CFG1:%x\r\n", SPI_CFG1(SPI4));
            usart_send_blocking_printf(USART3, "CFG2:%x\r\n", SPI_CFG2(SPI4));
            spi_sr = SPI2S_SR(SPI4);
            usart_send_blocking_printf(USART3, "SPI2S_SR: %x\r\n", spi_sr);
            if (spi_sr & SPI2S_SR_EOT) {
                spi_clear_flags(SPI4, SPI2S_IFCR_EOTC | SPI2S_IFCR_TXTFC);
            }
            usart_send_blocking_printf(USART3, "SPI2S_SR: %x\r\n", SPI2S_SR(SPI4));
            last_ts = mtime();

        }
    }
    return 0;
}
