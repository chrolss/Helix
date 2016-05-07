#include "mpu9150.h"


mpu9150::mpu9150(){
	initialize();
};

bool mpu9150::initialize(){
	this->i2cHandle = open("/dev/i2c-1", O_RDWR);
	memset(this->rxBuffer, 0, sizeof(this->rxBuffer));
	memset(this->txBuffer, 0, sizeof(this->txBuffer));
	this->opResult = ioctl(this->i2cHandle, I2C_TENBIT, 0); //setting the device address as 8-bit
	this->opResult = ioctl(this->i2cHandle, I2C_SLAVE, 0x68); //tell the i2c periheral which address to communicate with
	this->txBuffer[0] = 0x6B;
	this->txBuffer[1] = 1;
	this->opResult = write(this->i2cHandle, txBuffer, 2); //wake it up
	if (opResult != 2){
		printf("No ack bit on write!\n");
		return false;
	}
	return true;
}

void mpu9150::readAccelerations(){
	this->txBuffer[0] = 0x3B;	//0x3B is X_ACC_OUT_H
	this->opResult = write(this->i2cHandle, txBuffer, 1);
	if (opResult != 1){
		printf("no ack bit on read\n");
	}
	opResult = read(this->i2cHandle, rxBuffer, 6);
	this->xAcc = ((int16_t)rxBuffer[0]<<8) | rxBuffer[1];
	this->yAcc = ((int16_t)rxBuffer[2]<<8) | rxBuffer[3];
	this->zAcc = ((int16_t)rxBuffer[4]<<8) | rxBuffer[5];
	this->roll = atan2(yAcc,zAcc) * 180/M_PI;
	this->pitch = atan2(-xAcc, sqrt(yAcc*yAcc + zAcc*zAcc)) * 180/M_PI;
}

void mpu9150::getAccelerations(double *returnArray){
	readAccelerations();
	returnArray[0] = this->xAcc*aConv;
	returnArray[1] = this->yAcc*aConv;
	returnArray[2] = this->zAcc*aConv;
}

void mpu9150::getSensorReadings(double *returnArray){
	readAccelerations();
	returnArray[0] = this->xAcc*aConv;
	returnArray[1] = this->yAcc*aConv;
	returnArray[2] = this->zAcc*aConv;
	returnArray[3] = this->pitch;
	returnArray[4] = this->roll;
}

void mpu9150::readGyro(){
	this->txBuffer[0] = 0x43;
	this->opResult = write(this->i2cHandle, txBuffer, 1);
	if (opResult != 1){
		printf("no ack bit on read\n");
	}
	opResult = read(this->i2cHandle, rxBuffer, 2);
	this->xGyro = ((int16_t)rxBuffer[0]<<8) | rxBuffer[1];
	this->yGyro = ((int16_t)rxBuffer[2]<<8) | rxBuffer[3];
	this->zGyro = ((int16_t)rxBuffer[4]<<8) | rxBuffer[5];
}

void mpu9150::getGyro(double *returnArray){
	readGyro();
	returnArray[0] = this->xGyro;
	returnArray[1] = this->yGyro;
	returnArray[2] = this->zGyro;
}


void mpu9150::readTemp(){
	//nothing yet
}

double mpu9150::getTemp(){
	return 0.0;
}


