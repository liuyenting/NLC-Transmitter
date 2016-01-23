/* System header */
#include <stdint.h>
#include <stdbool.h>

/* Tiva Ware header */

/* Tiva Ware driver library */

/* Custom header */
#include "i2c.h"

#include "mcp4725.h"

uint8_t rx_buf[5];
#define current_dac_volt	(rx_buf[1] << 4) | (rx_buf[2] >> 4)
#define current_eeprom_volt	((rx_buf[3] & 0x0F) << 8) | rx_buf[4] 

void mcp4725_init(mcp4725 *dev, uint8_t dev_addr, i2c_port port, i2c_speed speed) {
	// Force degrade invalid speed mode.
	if(speed == I2C_FAST_MODE_PLUS)
		speed = I2C_FAST_MODE;
	
	// Store the value in the object.
	dev->addr = dev_addr; //(DEVICE_CODE << 4) | (dev_addr << 1);
	dev->port = port;
	i2c_set_speed(dev->port, speed);
	
	i2c_init(dev->port);
}

void mcp4725_set_voltage(mcp4725 *dev, uint16_t volt, bool write_eeprom) {
	// Convert the voltage to bit patterns.
	uint8_t upper_bits = volt >> 4;
	uint8_t lower_bits = (volt & 0x0F) << 4;
	
	if(write_eeprom) {
		i2c_send(dev->port, dev->addr, 
				 3, 
				 MCP4725_WRITE_DAC_AND_EEPROM,
				 upper_bits,
				 lower_bits);
	} else {
		i2c_send(dev->port, dev->addr, 
				 3, 
				 MCP4725_WRITE_DAC,
			     upper_bits,
				 lower_bits);
	}
}

/*
void mcp4725_read_voltage(mcp4725 *dev, uint16_t *dac, uint16_t *eeprom) {
	i2c_batch_receive(dev->addr | 0x01, 0x00, 5, rx_buf);
	*dac = current_dac_volt;
	*eeprom = current_eeprom_volt;
}
*/
