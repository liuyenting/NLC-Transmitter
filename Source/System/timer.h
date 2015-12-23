#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

void timer_init(uint32_t delay, void(*task)(void));
void timer_start(void);

// Function hook.
void (*periodic_task)(void);

#endif
