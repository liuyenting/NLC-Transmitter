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

void toggle_output() {
	if(set) {
		mcp4725_set_voltage(red, 0, false);
		set = false;
	} else {
		mcp4725_set_voltage(red, 2048, false);
		set = true;
	}
}

int main(void) {
	// Set the clock to run directly from the external oscillator.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
	
	timer_init(1000, &toggle_output);
	
	mcp4725_init(red, 0x00, I2C_STANDARD);
	
	while(1) {
		timer_start();
		//mcp4725_set_voltage(red, 0, false);
		//SysCtlDelay(16);
		//mcp4725_set_voltage(red, 2048, false);
		//SysCtlDelay(16);
	}
}
