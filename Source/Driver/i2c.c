#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"

#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "i2c.h"

void i2c_init(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);
	
	// Enable GPIO peripheral that contains I2C module 0.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
     
    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
 
    // Enable and initialize the I2C0 master module.  
	// Use the system clock for the I2C0 module.  
	// The last parameter sets the I2C data transfer rate. (T/F : 100/400kbps)
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);
     
    // Clear I2C FIFOs.
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;
}

void i2c_send(uint8_t slave_addr, uint8_t num_of_args, ...)
{
    // Tell the master module what address it will place on the bus.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
     
    va_list vargs;
    va_start(vargs, num_of_args);
     
    // Put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
     
    // If there is only one argument, we only need to send data once.
    if(num_of_args == 1) {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
         
        while(I2CMasterBusy(I2C0_BASE));
         
        va_end(vargs);
    } else {
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
         
        while(I2CMasterBusy(I2C0_BASE));
         
        // Send num_of_args-2 pieces of data, using the BURST_SEND_CONT command.
        for(uint8_t i = 1; i < (num_of_args - 1); i++)
        {
            // Put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
            // Send next data that was just placed into FIFO
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
     
            while(I2CMasterBusy(I2C0_BASE));
        }
     
        // Put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

        while(I2CMasterBusy(I2C0_BASE));
         
        va_end(vargs);
    }
}

uint32_t i2c_receive(uint32_t slave_addr, uint8_t reg)
{
    // Specify that we are writing (a register address) to the slave.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);
    // Specify register to be read
    I2CMasterDataPut(I2C0_BASE, reg);
    // Send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);
     
    while(I2CMasterBusy(I2C0_BASE));
     
    // Specify that we are going to read from slave device.
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true); 
    // Send control byte and read from the register we specified.
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
     
    while(I2CMasterBusy(I2C0_BASE));
     
    // Return data pulled from the specified register.
    return I2CMasterDataGet(I2C0_BASE);
}
