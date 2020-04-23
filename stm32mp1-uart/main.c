#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "clock.h"
#include "usart.h"
#include "led.h"

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

int main(void) {
    uint32_t last_ts = 0;
    uint32_t uart35clksel;
    uint32_t hsidiv;

    usart3_setup();

    systick_setup();
    led_setup();

    led_on();
    msleep(1000);
    led_off();
    msleep(1000);
    usart_send_blocking_str(USART3, "Hello World\r\n");

    uart35clksel = RCC_UART35CKSELR & (RCC_UART35CKSELR_UART35SRC_MASK << RCC_UART35CKSELR_UART35SRC_SHIFT);
    hsidiv = RCC_HSICFGR & (RCC_HSICFGR_HSIDIV_MASK << RCC_HSICFGR_HSIDIV_SHIFT);
    while(1) {
        if (last_ts + 5000 < mtime()) {
            switch (uart35clksel) {
            case RCC_UART35CKSELR_UART35SRC_PCLK1:
                led_blink(1);
                break;
            case RCC_UART35CKSELR_UART35SRC_PLL4_Q:
                led_blink(2);
                break;
            case RCC_UART35CKSELR_UART35SRC_HSI:
                led_blink(3);
                break;
            case RCC_UART35CKSELR_UART35SRC_CSI:
                led_blink(4);
                break;
            case RCC_UART35CKSELR_UART35SRC_HSE:
                led_blink(5);
                break;
            default:
                break;
            }
            msleep(2000);
            led_blink(hsidiv + 1);
            usart_send_blocking_str(USART3, "Hop\r\n");
            last_ts = mtime();

        }
    }
    return 0;
}
