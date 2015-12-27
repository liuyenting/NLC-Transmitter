/* System header */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Tiva Ware header */
#include "inc/tm4c123gh6pm.h"

/* Tiva Ware driver library */
#include "driverlib/sysctl.h"

/* Custom header */
#include "system/timer.h"
#include "driver/i2c.h"
#include "driver/mcp4725.h"

unsigned int malloc_mempool[0x100]; // 1KB memory pool.

bool set = false;

const uint8_t v_max = 4;
const uint16_t voltage_table[] = { 3000, 4000, 0, 4000 };
int v_index = 0;

bool wait_timeout;

void inc_voltage_index() {
	v_index = (v_index+1) % v_max;
	wait_timeout = false;
}

int main(void) {
	// Create memory pool for memory management routines.
	//init_mempool(malloc_mempool, sizeof(malloc_mempool));
	
	//mcp4725 *red = NULL; //, *green = NULL, *blue = NULL;
	mcp4725 red;
	
	// Set the clock to run directly from the external oscillator.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
	
	//timer_init(1000, &inc_voltage_index);
	
	mcp4725_init(&red, 0x60, I2C_STANDARD);
	
	while(1) {
		//wait_timeout = true;
		//timer_start();
		//while(wait_timeout);
		//mcp4725_set_voltage(red, voltage_table[v_index], false);
		
		//mcp4725_set_voltage(&red, 0, false);
		//SysCtlDelay(SysCtlClockGet()/3);
		//mcp4725_set_voltage(&red, 2048, false);
		//SysCtlDelay(SysCtlClockGet()/3);
		
		mcp4725_set_voltage(&red, voltage_table[v_index], false);
		inc_voltage_index();
		SysCtlDelay(SysCtlClockGet()/3000 * 1/10); // 1kHz * freq
	}
}
