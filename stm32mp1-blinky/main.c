#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "api.h"
#include "api-asm.h"

#define RCC_LED7  RCC_GPIOH
#define PORT_LED7 GPIOH
#define PIN_LED7  7

#define LITTLE_BIT 100000

int main(void) {
    rcc_periph_clock_enable(RCC_LED7);
    gpio_mode_setup(PORT_LED7, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED7);
    gpio_set(PORT_LED7, PIN_LED7);
    while(1) {
        /* wait a little bit */
        for (int i = 0; i < LITTLE_BIT; i++) {
            __asm__("nop");
        }
        gpio_toggle(PORT_LED7, PIN_LED7);
    }
    return 0;
}
