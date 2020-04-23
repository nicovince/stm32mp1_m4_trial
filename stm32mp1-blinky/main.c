#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "api.h"
#include "api-asm.h"
#include "led.h"

#define RCC_LED7  RCC_GPIOH
#define PORT_LED7 GPIOH
#define PIN_LED7  7
#define GPIO_LED7 (1 << PIN_LED7)

#define LITTLE_BIT 10000000

int main(void) {
    led_setup();
    led_on();
    while(1) {
        /* wait a little bit */
        for (int i = 0; i < LITTLE_BIT; i++) {
            __asm__("nop");
        }
        led_toggle();
    }
    return 0;
}
