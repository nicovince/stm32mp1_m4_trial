#include <libopencm3/cm3/systick.h>

#include "clock.h"

#define LITTLE_BIT 10000000

void sys_tick_handler(void);
static volatile uint32_t system_millis = 0;

void sys_tick_handler(void)
{
    system_millis++;
}

void msleep(uint32_t sleep_ms)
{
    uint32_t start_ms = system_millis;
    while (start_ms + sleep_ms > system_millis);
}

uint32_t mtime(void)
{
    return system_millis;
}

void dirty_wait(void)
{
    /* wait a little bit */
    for (int i = 0; i < LITTLE_BIT; i++) {
        __asm__("nop");
    }
}

void systick_setup(void)
{
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(209000); /* ahb runs at 209MHz, this value gives a 1ms systick */
    systick_interrupt_enable();
    systick_counter_enable();
}

