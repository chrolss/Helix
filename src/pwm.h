#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "i2c.h"
#include "math.h"

#ifndef PWM_H_
#define PWM_H_


// Register addresses
#define MODE1 0x00
#define MODE2 0x01
#define LED0_ON_L	0x06
#define LED0_ON_H	0x07
#define LED0_OFF_L	0x08
#define LED0_OFF_H	0x09
#define LED1_ON_L	0x0A
#define LED1_ON_H	0x0B
#define LED1_OFF_L	0x0C
#define LED1_OFF_H	0x0D
#define LED2_ON_L	0x0E
#define LED2_ON_H	0x0F
#define LED2_OFF_L	0x10
#define LED2_OFF_H	0x11
#define LED3_ON_L	0x12
#define LED3_ON_H	0x13
#define LED3_OFF_L	0x14
#define LED3_OFF_H	0x15

// Register Values
#define NORMAL_MODE 0x00 	// can read and write to PWM channels
#define SLEEP_MODE 0x10		// cannot read nor write to pwm channels
#define TOTEM_POLE 0x04		//standard mode (?)



class pwm{
public:
	pwm();
	void initialize();
	void setDutyCycle(double _duty);
	void setQuadDutyCycle(double* _duty);
	void setFrequency(int _freq);
	void closePWM();
	void startQuadEngines();
private:
	int devAdress = 0x40; //i2c address to Adafruit PWM hat
	int pinNr;
	int i2cHandle;
	int opRes;
	char txBuffer[32];
	char rxBuffer[32];
};

#endif
