#include <stdbool.h>
#include <stdint.h>

#include "inc/tm4c123gh6pm.h"

#include "driverlib/sysctl.h"

#include "driver/i2c.h"

int main(void) {
	// Set the clock to run directly from the external oscillator.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
	
	i2c_init();
	
	while(1) {
		i2c_send(0x60, 3, 0x40, 0x00, 0x00);
		SysCtlDelay(16);
		i2c_send(0x60, 3, 0x40, 0xFF, 0xF0);
		SysCtlDelay(16);
	}
}
