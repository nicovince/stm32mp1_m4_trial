#ifndef __CLOCK_H__
#define __CLOCK_H__

void systick_setup(void);
void msleep(uint32_t sleep_ms);
uint32_t mtime(void);
void dirty_wait(void);

#endif /* __CLOCK_H__ */

