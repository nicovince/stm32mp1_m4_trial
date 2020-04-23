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
    systick_setup();

    led_setup();
    led_off();
    msleep(500);
    led_on();
    msleep(500);
    led_off();

    usart3_setup();

    usart_send_printf(USART3, "Hello World\r\n");

    while(1) {
        if (last_ts + 3000 < mtime()) {
            usart_send_printf(USART3, "mtime: %d\r\n", mtime());
            last_ts = mtime();

        }
    }
    return 0;
}
