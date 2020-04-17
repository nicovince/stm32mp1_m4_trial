#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

#include "api.h"
#include "api-asm.h"

#define RCC_LED7  RCC_GPIOH
#define PORT_LED7 GPIOH
#define PIN_LED7  7
#define GPIO_LED7 (1 << PIN_LED7)

#define LITTLE_BIT 10000000

void sys_tick_handler(void);
static volatile uint32_t system_millis = 0;
void sys_tick_handler(void)
{
    system_millis++;
}

static void msleep(uint32_t sleep_ms)
{
    uint32_t start_ms = system_millis;
    while (start_ms + sleep_ms > system_millis);
}

static void dirty_wait(void)
{
    /* wait a little bit */
    for (int i = 0; i < LITTLE_BIT; i++) {
        __asm__("nop");
    }
}

static void systick_setup(void)
{
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(209000); /* ahb runs at 209MHz, this value gives a 1ms systick */
    systick_interrupt_enable();
    systick_counter_enable();
}

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
        if (last_ts + 1000 < system_millis) {
            last_ts = system_millis;
            gpio_toggle(PORT_LED7, GPIO_LED7);
        }
    }
    return 0;
}
