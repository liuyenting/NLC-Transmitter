/* System header */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Tiva Ware header */
#include "inc/tm4c123gh6pm.h"

/* Tiva Ware driver library */
#include "driverlib/sysctl.h"

/* Custom header */
#include "system/timer.h"
#include "driver/i2c.h"
#include "driver/mcp4725.h"

mcp4725 *red = NULL; //, *green = NULL, *blue = NULL;
bool set = false;

const uint16_t voltage_table[5] = { 0, 512, 1024, 1536, 2048 };
int v_index = 0;

bool wait_timeout;

void inc_voltage_index() {
	v_index = (v_index+1) % 5;
	wait_timeout = false;
}

int main(void) {
	// Set the clock to run directly from the external oscillator.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
	
	timer_init(1000, &inc_voltage_index);
	
	mcp4725_init(red, 0x00, I2C_STANDARD);
	
	while(1) {
		wait_timeout = true;
		timer_start();
		while(wait_timeout);
		mcp4725_set_voltage(red, voltage_table[v_index], false);
		
		//mcp4725_set_voltage(red, 0, false);
		//SysCtlDelay(16);
		//mcp4725_set_voltage(red, 2048, false);
		//SysCtlDelay(16);
	}
}
