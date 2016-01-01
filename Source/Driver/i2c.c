/* System header */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

/* Tiva Ware header */
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

/* Tiva Ware driver library */
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "i2c.h"

i2c_port_info port_info[4] = {
	{
		.port = GPIO_PORTB_BASE,
		.sda_pin = GPIO_PIN_3,
		.scl_pin = GPIO_PIN_2,
		.sda_reg = GPIO_PB3_I2C0SDA,
		.scl_reg = GPIO_PB2_I2C0SCL,
		.i2c_periph = SYSCTL_PERIPH_I2C0,
		.gpio_periph = SYSCTL_PERIPH_GPIOB,
		.base_addr = I2C0_BASE,
		.speed = I2C_STANDARD
	},
	{
		.port = GPIO_PORTA_BASE,
		.sda_pin = GPIO_PIN_7,
		.scl_pin = GPIO_PIN_6,
		.sda_reg = GPIO_PA7_I2C1SDA,
		.scl_reg = GPIO_PA6_I2C1SCL,
		.i2c_periph = SYSCTL_PERIPH_I2C1,
		.gpio_periph = SYSCTL_PERIPH_GPIOA,
		.base_addr = I2C1_BASE,
		.speed = I2C_STANDARD
	},
	{
		.port = GPIO_PORTE_BASE,
		.sda_pin = GPIO_PIN_5,
		.scl_pin = GPIO_PIN_4,
		.sda_reg = GPIO_PE5_I2C2SDA,
		.scl_reg = GPIO_PE4_I2C2SCL,
		.i2c_periph = SYSCTL_PERIPH_I2C2,
		.gpio_periph = SYSCTL_PERIPH_GPIOE,
		.base_addr = I2C2_BASE,
		.speed = I2C_STANDARD
	},
	{
		.port = GPIO_PORTD_BASE,
		.sda_pin = GPIO_PIN_1,
		.scl_pin = GPIO_PIN_0,
		.sda_reg = GPIO_PD1_I2C3SDA,
		.scl_reg = GPIO_PD0_I2C3SCL,
		.i2c_periph = SYSCTL_PERIPH_I2C3,
		.gpio_periph = SYSCTL_PERIPH_GPIOD,
		.base_addr = I2C3_BASE,
		.speed = I2C_STANDARD
	}
};

void i2c_busy_wait(uint32_t base) {
	while(I2CMasterBusy(base));
}

void i2c_init(i2c_port port) {
	SysCtlPeripheralEnable(port_info[port].i2c_periph);
	SysCtlPeripheralReset(port_info[port].i2c_periph);
	
	// Enable GPIO peripheral that contains I2C module 0.
    SysCtlPeripheralEnable(port_info[port].gpio_periph);
     
    // Select the I2C function for these pins.
	GPIOPinConfigure(port_info[port].sda_reg);
    GPIOPinTypeI2C(port_info[port].port, port_info[port].sda_pin);
	GPIOPinConfigure(port_info[port].scl_reg);
	GPIOPinTypeI2CSCL(port_info[port].port, port_info[port].scl_pin);
     
    // Enable and initialize the I2C0 master module.  
	// Use the system clock for the I2C0 module.  
	// The last parameter sets the I2C data transfer rate. (F/T : 100/400kbps)
	switch(port_info[port].speed) {
		case I2C_STANDARD:
			I2CMasterInitExpClk(port_info[port].base_addr, SysCtlClockGet(), false);
			break;
		case I2C_FAST_MODE:
			I2CMasterInitExpClk(port_info[port].base_addr, SysCtlClockGet(), true);
			break;
		case I2C_FAST_MODE_PLUS:
			break;
		case I2C_HIGH_SPEED_MODE: // TODO: Manual register control needed, I2CMRIS.
			break;
	}
     
    // Clear I2C FIFOs.
    HWREG(port_info[port].base_addr + I2C_O_FIFOCTL) = 80008000;
}

void i2c_set_speed(i2c_port port, i2c_speed speed) {
	port_info[port].speed = speed;
}

void i2c_send(i2c_port port, uint8_t slave_addr, uint8_t num_of_args, ...)
{
	uint32_t base_addr = port_info[port].base_addr;
	
    // Tell the master module what address it will place on the bus.
    I2CMasterSlaveAddrSet(base_addr, slave_addr, false);
     
    va_list vargs;
    va_start(vargs, num_of_args);
     
    // Put data to be sent into FIFO
    I2CMasterDataPut(base_addr, va_arg(vargs, uint32_t));
     
    // If there is only one argument, we only need to send data once.
    if(num_of_args == 1) {
        I2CMasterControl(base_addr, I2C_MASTER_CMD_SINGLE_SEND);
        i2c_busy_wait(base_addr);
         
        va_end(vargs);
    } else {
        I2CMasterControl(base_addr, I2C_MASTER_CMD_BURST_SEND_START);
        i2c_busy_wait(base_addr);
         
        // Send num_of_args-2 pieces of data, using the BURST_SEND_CONT command.
        for(uint8_t i = 1; i < (num_of_args-1); i++)
        {
            // Put next piece of data into I2C FIFO
            I2CMasterDataPut(base_addr, va_arg(vargs, uint32_t));
			
            // Send next data that was just placed into FIFO
            I2CMasterControl(base_addr, I2C_MASTER_CMD_BURST_SEND_CONT);
			i2c_busy_wait(base_addr);
        }
     
        // Put last piece of data into I2C FIFO
        I2CMasterDataPut(base_addr, va_arg(vargs, uint32_t));
        I2CMasterControl(base_addr, I2C_MASTER_CMD_BURST_SEND_FINISH);
		i2c_busy_wait(base_addr);
		
        va_end(vargs);
    }
}

/*
uint32_t i2c_receive(uint32_t slave_addr, uint8_t reg)
{
    // Specify that we are writing (a register address) to the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
    // Specify register to be read
    I2CMasterDataPut(I2C0_BASE, reg);
    // Send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    i2c_busy_wait(I2C0_BASE);
     
    // Specify that we are going to read from slave device.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true); 
    // Send control byte and read from the register we specified.
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    i2c_busy_wait(I2C0_BASE);
     
    // Return data pulled from the specified register.
    return I2CMasterDataGet(I2C0_BASE);
}

void i2c_batch_receive(uint32_t slave_addr, uint8_t cmd, uint8_t num_of_dat, uint8_t *buf) {
	if(cmd != 0x00) {
		I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
		I2CMasterDataPut(I2C0_BASE, cmd);
		
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
		i2c_busy_wait(I2C0_BASE);
	}
	
	// Specify to read from the slave device.
	I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);
	// Start batch read.
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
	i2c_busy_wait(I2C0_BASE);
	
	for(uint8_t i = 0; i < (num_of_dat-1); i++) {
		buf[i] = I2CMasterDataGet(I2C0_BASE);
		
		I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
		i2c_busy_wait(I2C0_BASE);
	}
	
	// Receive last piece of data.
	buf[num_of_dat-1] = I2CMasterDataGet(I2C0_BASE);
	
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);	
	i2c_busy_wait(I2C0_BASE);
}
*/
