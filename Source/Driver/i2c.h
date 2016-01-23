#ifndef __I2C_H__
#define __I2C_H__

/* System header */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

/* Tiva Ware header */
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

/* Tiva Ware driver library */
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

typedef enum {
	I2C_PORT0 = 0,
	I2C_PORT1,
	I2C_PORT2,
	I2C_PORT3
} i2c_port;

typedef enum {
	I2C_STANDARD, 
	I2C_FAST_MODE,
	I2C_FAST_MODE_PLUS,
	I2C_HIGH_SPEED_MODE
} i2c_speed;

typedef struct {
	// Physical location.
	uint32_t port;
	uint32_t sda_pin, scl_pin;
	
	// Peripheral register address.
	uint32_t i2c_periph, gpio_periph;
	uint32_t sda_reg, scl_reg;
	
	// Base register address.
	uint32_t base_addr;
	
	// Custom speed definition.
	i2c_speed speed;
} i2c_port_info;

void i2c_init(i2c_port port);
void i2c_set_speed(i2c_port port, i2c_speed speed);
void i2c_send(i2c_port port, uint8_t slave_addr, uint8_t num_of_args, ...);

/*
uint32_t i2c_receive(uint32_t slave_addr, uint8_t reg);
void i2c_batch_receive(uint32_t slave_addr, uint8_t cmd, uint8_t num_of_dat, uint8_t *buf);
*/

#endif
