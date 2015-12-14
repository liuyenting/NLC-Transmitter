#ifndef __I2C_H__
#define __I2C_H__

#include <stdarg.h>
#include <stdint.h>

typedef enum {
	I2C_STANDARD, 
	I2C_FAST_MODE,
	I2C_FAST_MODE_PLUS,
	I2C_HIGH_SPEED_MODE
} i2c_speed;

void i2c_init(i2c_speed speed);
void i2c_send(uint8_t slave_addr, uint8_t num_of_args, ...);
uint32_t i2c_receive(uint32_t slave_addr, uint8_t reg);
void i2c_batch_receive(uint32_t slave_addr, uint8_t cmd, uint8_t num_of_dat, uint8_t *buf);

#endif
