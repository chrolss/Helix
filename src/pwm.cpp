#include "pwm.h"

pwm::pwm(){
	initialize();
}
void pwm::initialize(){
	this->i2cHandle = open("/dev/i2c-1", O_RDWR);
	memset(this->rxBuffer, 0, sizeof(this->rxBuffer));
	memset(this->txBuffer, 0, sizeof(this->txBuffer));
	this->opRes = ioctl(this->i2cHandle, I2C_TENBIT, 0); //setting the device address as 8-bit
	this->opRes = ioctl(this->i2cHandle, I2C_SLAVE, 0x40); //tell the i2c periheral which address to communicate with

	//put some code here that sets the ON/OFF_L/H depending on
	//pin number
}

void pwm::setDutyCycle(double _duty){
	double val = 4096.0*(_duty/100);	//duty percentage to 4096 bit value
	txBuffer[0] = LED0_ON_L;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED0_ON_H;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED0_OFF_L;
	txBuffer[1] = (int)val&0xFF;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED0_OFF_H;
	txBuffer[1] = (int)val >> 8;
	opRes = write(this->i2cHandle, txBuffer, 2);
}

void pwm::setQuadDutyCycle(double* _duty){
	double RF = 4096.0*(_duty[0]/100.0);	//duty percentage to 4096 bit value
	double RR = 4096.0*(_duty[1]/100.0);
	double LR = 4096.0*(_duty[2]/100.0);
	double LF = 4096.0*(_duty[3]/100.0);
	//Right front
	txBuffer[0] = LED0_ON_L;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED0_ON_H;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED0_OFF_L;
	txBuffer[1] = (int)RF&0xFF;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED0_OFF_H;
	txBuffer[1] = (int)RF >> 8;
	opRes = write(this->i2cHandle, txBuffer, 2);
	//Right rear
	txBuffer[0] = LED1_ON_L;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED1_ON_H;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED1_OFF_L;
	txBuffer[1] = (int)RR&0xFF;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED1_OFF_H;
	txBuffer[1] = (int)RR >> 8;
	opRes = write(this->i2cHandle, txBuffer, 2);
	//Left rear
	txBuffer[0] = LED2_ON_L;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED2_ON_H;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED2_OFF_L;
	txBuffer[1] = (int)LR&0xFF;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED2_OFF_H;
	txBuffer[1] = (int)LR >> 8;
	opRes = write(this->i2cHandle, txBuffer, 2);
	//left front
	txBuffer[0] = LED3_ON_L;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED3_ON_H;
	txBuffer[1] = 0;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED3_OFF_L;
	txBuffer[1] = (int)LF&0xFF;
	opRes = write(this->i2cHandle, txBuffer, 2);
	txBuffer[0] = LED3_OFF_H;
	txBuffer[1] = (int)LF >> 8;
	opRes = write(this->i2cHandle, txBuffer, 2);
}

void pwm::setFrequency(int _freq){
	rxBuffer[0] = 0x00;
	int oldMode = read(this->i2cHandle, rxBuffer,1);
	int newMode = (oldMode & 0x7F) | 0x10;
	txBuffer[0] = newMode;
	opRes = write(this->i2cHandle, txBuffer,1);
	double val = 25000000.0/4096.0;
	double preVal = val/(double)_freq;
	preVal = preVal - 1;
	int setVal = floor(preVal + 0.5);
	txBuffer[0] = 0xFE;
	txBuffer[1] = setVal;
	opRes = write(this->i2cHandle, txBuffer, 2);
	if (opRes != 2){
		printf("Error when writing frequency\n");
	}
	txBuffer[0] = 0x00;
	txBuffer[1] = oldMode;
	opRes = write(this->i2cHandle, txBuffer, 2);
	usleep(10000);
	txBuffer[0] = 0x00;
	txBuffer[1] = oldMode | 0x80;
	opRes = write(this->i2cHandle, txBuffer, 2);

}
