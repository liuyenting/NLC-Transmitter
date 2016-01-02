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
#include "def/color.h"

bool set = false;

const uint8_t msg_max = 6;
const uint16_t msg_table[] = { 0, 2, 4, 3, 4, 1 };
int msg_index = 0;

bool wait_timeout;

void inc_msg_index() {
	msg_index = (msg_index+1) % msg_max;
	wait_timeout = false;
}

uint16_t get_abs_voltage(uint8_t channel, uint8_t rel_level) {
	return abs_level[channel][rel_level];
}

int main(void) {
	mcp4725 channels[3];
	
	// Set the clock to run directly from the external oscillator.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL | SYSCTL_OSC_INT | SYSCTL_XTAL_16MHZ);
	
	//timer_init(1000, &inc_voltage_index);
	
	// Initialize the DACs.
	mcp4725_init(&(channels[CH_RED]),	0x60, I2C_PORT0, I2C_STANDARD);
	mcp4725_init(&(channels[CH_GREEN]), 0x60, I2C_PORT1, I2C_STANDARD);
	mcp4725_init(&(channels[CH_BLUE]),	0x60, I2C_PORT2, I2C_STANDARD);
	
	uint8_t curr_channel = CH_RED;
	
	for(uint16_t i = 0; i <= 4000; i += 100)
		mcp4725_set_voltage(&(channels[curr_channel]), i, false);
	
	return 0;
	
	while(1) {
		//wait_timeout = true;
		//timer_start();
		//while(wait_timeout);
		//mcp4725_set_voltage(red, voltage_table[v_index], false);
		
		//mcp4725_set_voltage(&red, 0, false);
		//SysCtlDelay(SysCtlClockGet()/3);
		//mcp4725_set_voltage(&red, 2048, false);
		//SysCtlDelay(SysCtlClockGet()/3);
		
		mcp4725_set_voltage(&(channels[curr_channel]), 
							get_abs_voltage(curr_channel, msg_table[msg_index]), 
							false);
		inc_msg_index();
		SysCtlDelay(SysCtlClockGet()/3000 * 500); // 1kHz * duration
	}
}
