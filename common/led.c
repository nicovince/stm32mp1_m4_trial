#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "led.h"
#include "clock.h"

#define RCC_LED7  RCC_GPIOH
#define PORT_LED7 GPIOH
#define PIN_LED7  7
#define GPIO_LED7 (1 << PIN_LED7)

void led_setup(void)
{
    rcc_periph_clock_enable(RCC_LED7);
    gpio_mode_setup(PORT_LED7, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_LED7);
    gpio_set_output_options(PORT_LED7, GPIO_OTYPE_PP,
                            GPIO_OSPEED_25MHZ,  GPIO_LED7);
}

void led_toggle(void)
{
    gpio_toggle(PORT_LED7, GPIO_LED7);
}

void led_blink(uint8_t n)
{
    for (uint8_t i = 0; i < n; ++i)
    {
        gpio_set(PORT_LED7, GPIO_LED7);
        msleep(200);
        gpio_clear(PORT_LED7, GPIO_LED7);
        msleep(200);
    }
}

