#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "clock.h"

#include "api.h"
#include "api-asm.h"

#define RCC_LED7  RCC_GPIOH
#define PORT_LED7 GPIOH
#define PIN_LED7  7
#define GPIO_LED7 (1 << PIN_LED7)

int main(void) {
    uint32_t last_ts = 0;
    systick_setup();
    rcc_periph_clock_enable(RCC_LED7);
    gpio_mode_setup(PORT_LED7, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LED7);
    gpio_set_output_options(PORT_LED7, GPIO_OTYPE_PP,
                            GPIO_OSPEED_25MHZ,  GPIO_LED7);
    gpio_set(PORT_LED7, GPIO_LED7);
    msleep(1000);
    gpio_clear(PORT_LED7, GPIO_LED7);
    msleep(1000);
    while(1) {
        if (last_ts + 1000 < mtime()) {
            last_ts = mtime();
            gpio_toggle(PORT_LED7, GPIO_LED7);
        }
    }
    return 0;
}
