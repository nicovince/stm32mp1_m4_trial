#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>

#include "clock.h"
#include "usart.h"
#include "spi.h"
#include "led.h"

#include "api.h"
#include "api-asm.h"

int main(void) {
    uint32_t last_ts = 0;
    uint32_t spi_data;
    uint32_t cnt = 0;
    uint32_t spi_sr;

    usart3_setup();

    systick_setup();
    led_setup();

    spi4_setup();
    usart_send_printf(USART3, "Hello World\r\n");
    usart_send_printf(USART3, "CR1:%x\r\n", SPI2S_CR1(SPI4));
    usart_send_printf(USART3, "CFG1:%x\r\n", SPI_CFG1(SPI4));
    usart_send_printf(USART3, "CFG2:%x\r\n", SPI_CFG2(SPI4));
    usart_send_printf(USART3, "SPI2S_SR: %x\r\n", SPI2S_SR(SPI4));

    while(1) {
        if (last_ts + 5000 < mtime()) {
            usart_send_printf(USART3, "----- [%d] -----\r\n", mtime());
            led_toggle();
            spi_write8(SPI4, cnt++);
            spi_write8(SPI4, cnt++);
            spi_write8(SPI4, cnt++);
            usart_send_printf(USART3, "spidata = %d\r\n", mtime(), spi_data);
            usart_send_printf(USART3, "CR1:%x\r\n", SPI2S_CR1(SPI4));
            usart_send_printf(USART3, "CR2:%x\r\n", SPI_CR2(SPI4));
            usart_send_printf(USART3, "CFG1:%x\r\n", SPI_CFG1(SPI4));
            usart_send_printf(USART3, "CFG2:%x\r\n", SPI_CFG2(SPI4));
            spi_sr = SPI2S_SR(SPI4);
            usart_send_printf(USART3, "SPI2S_SR: %x\r\n", spi_sr);
            if (spi_sr & SPI2S_SR_EOT) {
                spi_clear_flags(SPI4, SPI2S_IFCR_EOTC | SPI2S_IFCR_TXTFC);
            }
            usart_send_printf(USART3, "SPI2S_SR: %x\r\n", SPI2S_SR(SPI4));
            last_ts = mtime();

        }
    }
    return 0;
}
