#include "i2c.h"

i2c::i2c(int _devAddress){
	this->devAddress = _devAddress; //test register, not real. need to add devAdress in private field of i2c.h
	this->i2cHandle = open("/dev/i2c-1", O_RDWR);
	memset(this->rxBuffer, 0, sizeof(this->rxBuffer));
	memset(this->txBuffer, 0, sizeof(this->txBuffer));
	this->tenBitAddress = 0;
	this->opResult = ioctl(this->i2cHandle, I2C_TENBIT, tenBitAddress); //setting the device address as 8-bit
	this->opResult = ioctl(this->i2cHandle, I2C_SLAVE, devAddress); //tell the i2c periheral which address to communicate with
}

int i2c::readAcc(){
	this->txBuffer[0] = 0x3B;
	this->opResult = write(this->i2cHandle, txBuffer, 1);
	if (opResult != 1){
		printf("no ack bit on read\n");
		return -1;
	}
	opResult = read(this->i2cHandle, rxBuffer, 2);
	return ((int16_t)rxBuffer[0]<<8) | rxBuffer[1];
}

int i2c::readGyro(){
	this->txBuffer[0] = 0x43;
	this->opResult = write(this->i2cHandle, txBuffer, 1);
	if (opResult != 1){
		printf("no ack bit on read\n");
		return -1;
	}
	opResult = read(this->i2cHandle, rxBuffer, 2);
	return ((int16_t)rxBuffer[0]<<8) | rxBuffer[1];
}

int i2c::readReg(char _register){
	this->txBuffer[0] = _register;
    this->opResult = write(this->i2cHandle, txBuffer, 1);
    if (opResult != 1){
    	printf("No ack bit on read");
    }
    opResult = read(this->i2cHandle, this->rxBuffer, 1);
    return (int)rxBuffer[0];
}

int i2c::writeReg(char _register, char _msg, int _bit){
	this->txBuffer[0] = _register;
	this->txBuffer[1] = _msg;
	this->opResult = write(this->i2cHandle, txBuffer, _bit);
	if (opResult != _bit){
		printf("No ack bit on write!\n");
		return opResult;
	}
	return opResult;
	}

