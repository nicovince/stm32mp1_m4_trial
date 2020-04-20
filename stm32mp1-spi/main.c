#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "clock.h"
#include "usart.h"

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
    uint32_t uart35clksel;
    uint32_t hsidiv;

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
    gpio_set(PORT_LED7, GPIO_LED7);
    msleep(1000);
    gpio_clear(PORT_LED7, GPIO_LED7);
    msleep(1000);
    usart_send_blocking_str(USART3, "Hello World\r\n");

    uart35clksel = RCC_UART35CKSELR & (RCC_UART35CKSELR_UART35SRC_MASK << RCC_UART35CKSELR_UART35SRC_SHIFT);
    hsidiv = RCC_HSICFGR & (RCC_HSICFGR_HSIDIV_MASK << RCC_HSICFGR_HSIDIV_SHIFT);
    while(1) {
        if (last_ts + 5000 < mtime()) {
            switch (uart35clksel) {
            case RCC_UART35CKSELR_UART35SRC_PCLK1:
                blink(1);
                break;
            case RCC_UART35CKSELR_UART35SRC_PLL4_Q:
                blink(2);
                break;
            case RCC_UART35CKSELR_UART35SRC_HSI:
                blink(3);
                break;
            case RCC_UART35CKSELR_UART35SRC_CSI:
                blink(4);
                break;
            case RCC_UART35CKSELR_UART35SRC_HSE:
                blink(5);
                break;
            default:
                break;
            }
            msleep(2000);
            blink(hsidiv + 1);
            usart_send_blocking_str(USART3, "Hop\r\n");
            last_ts = mtime();

        }
    }
    return 0;
}
