#ifndef __I2C_H__
#define __I2C_H__

#include <stdarg.h>
#include <stdint.h>

void i2c_init(void);
void i2c_send(uint8_t slave_addr, uint8_t num_of_args, ...);
uint32_t i2c_receive(uint32_t slave_addr, uint8_t reg);

#endif
