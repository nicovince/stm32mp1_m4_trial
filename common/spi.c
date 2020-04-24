#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "spi.h"

// DEBUG
#include "usart.h"
#include <libopencm3/stm32/usart.h>
// DEBUG

#define SPI4_PORT      GPIOE
#define SPI4_NSS_GPIO  (1 << 11)
#define SPI4_SCK_GPIO  (1 << 12)
#define SPI4_MISO_GPIO (1 << 13)
#define SPI4_MOSI_GPIO (1 << 14)
#define SPI4_AF        GPIO_AF5

void spi4_setup(void)
{
    uint16_t spi_out_gpios = (SPI4_NSS_GPIO | SPI4_SCK_GPIO |  SPI4_MOSI_GPIO);
    uint16_t spi_gpios = (spi_out_gpios | SPI4_MISO_GPIO);

    rcc_periph_clock_enable(RCC_SPI4);
    spi_reset(SPI4);
    rcc_periph_clock_enable(RCC_GPIOE);

    gpio_mode_setup(SPI4_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, spi_gpios);
    gpio_set_output_options(SPI4_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, spi_out_gpios);
    gpio_set_af(SPI4_PORT, SPI4_AF, spi_gpios);
    usart_send_blocking_printf(USART3, "---CFG1:%x\r\n", SPI_CFG1(SPI4));

    spi_init_master(SPI4, SPI_CFG1_MBR_MCK_DIV256,
                    SPI_CFG2_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CFG2_CPHA_CLK_TRANSITION_1,
                    SPI_CFG2_MSBFIRST);
    spi_disable_software_slave_management(SPI4);
    spi_set_transfer_size(SPI4, 3, 0);
    spi_set_interdata_idleness(SPI4, 2);
    spi_set_communication_mode(SPI4, SPI_CFG2_COMM_FD);
    spi_set_data_size(SPI4, 8-1);
    spi_enable(SPI4);
}

