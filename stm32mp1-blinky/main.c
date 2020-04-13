#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "api.h"
#include "api-asm.h"

#define RCC_LED7  RCC_GPIOH
#define PORT_LED7 GPIOH
#define PIN_LED7  7
#define GPIO_LED7 (1 << PIN_LED7)

#define LITTLE_BIT 10000000

int main(void) {
    rcc_periph_clock_enable(RCC_LED7);
    gpio_mode_setup(PORT_LED7, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LED7);
    gpio_set_output_options(PORT_LED7, GPIO_OTYPE_PP,
                            GPIO_OSPEED_25MHZ,  GPIO_LED7);
    gpio_set(PORT_LED7, GPIO_LED7);
    while(1) {
        /* wait a little bit */
        for (int i = 0; i < LITTLE_BIT; i++) {
            __asm__("nop");
        }
        gpio_toggle(PORT_LED7, GPIO_LED7);
    }
    return 0;
}
