#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "math.h"

#ifndef MPU9150_H_
#define MPU9150_H_

class mpu9150{
public:
	mpu9150();
	void getAccelerations(double *returnArray);
	void getSensorReadings(double *returnArray);
	void getGyro(double *returnArray);
	double getTemp();
	double getRoll(){return roll;}
	double getPitch(){return pitch;}
private:
	int i2cHandle;
	char txBuffer[32];
	char rxBuffer[32];
	int16_t xAcc;
	int16_t yAcc;
	int16_t zAcc;
	double accs [3];
	double aConv = 2.0/32768.0;
	int16_t xGyro;
	int16_t yGyro;
	int16_t zGyro;
	double gyros [3];
	double roll;
	double pitch;
	bool initialize();
	void readAccelerations();
	void readGyro();
	void readTemp();
	int opResult;
};

#endif
