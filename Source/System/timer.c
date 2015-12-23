/* System header */
#include <stdint.h>
#include <stdbool.h>

/* Tiva Ware header */
#include "inc/hw_memmap.h"

/* Tiva Ware driver library */
#include "driverlib/timer.h"

#include "timer.h"

void timer_init(uint32_t delay, void(*task)(void))
{
	//
    // Configure TimerA as a half-width one-shot timer, and TimerB as a
    // half-width edge capture counter.
    //
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT |
                                 TIMER_CFG_B_CAP_COUNT));
	
	// Link the hook.
	periodic_task = task;
	
	// Set the count time for the one-shot timer (TimerA).
    TimerLoadSet(TIMER0_BASE, TIMER_A, delay);
}

void timer_start(void) {
	TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0IntHandler(void)
{
    // Clear the timer interrupt.
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
 
    // Get the counter value.
    uint32_t delay = TimerValueGet(TIMER0_BASE, TIMER_A);
	
	// Execute the hooked function.
	(*periodic_task)();
 
    // Reset the counter value.
    TimerLoadSet(TIMER0_BASE, TIMER_A, delay);
}
