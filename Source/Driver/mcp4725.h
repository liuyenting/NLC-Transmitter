#ifndef __MCP4725_H__
#define __MCP4725_H__

/* System header */
#include <stdint.h>
#include <stdbool.h>

/* Custom header */
#include "i2c.h"

#define DEVICE_CODE						0x0C

#define MCP4725_WRITE_DAC				0x40
#define MCP4725_WRITE_DAC_AND_EEPROM	0x60
#define MCP4725_WRITE_FAST_MODE			0x00

/*
Fast mode (repeat sequence):
	< 0  0  PD1 PD0 D11 D10 D9  D8 >
	< D7 D6 D5  D4  D3  D2  D1  D0 >

Standard (one-shot):
	<0   1   C0 X  X  PD1 PD0 X  >
	<D11 D10 D9 D8 D7 D6  D5  D4 >
	<D3  D2  D1 D0 X  X   X   X  >
	C0: 1/0 = DAC&EEPROM/DAC
*/

typedef struct {
	uint8_t addr;
	i2c_speed speed;
} mcp4725;

void mcp4725_init(mcp4725 *dev, uint8_t dev_addr, i2c_speed speed);
void mcp4725_set_voltage(mcp4725 *dev, uint16_t volt, bool write_eeprom);
void mcp4725_read_voltage(mcp4725 *dev, uint16_t *dac, uint16_t *eeprom);

#endif
