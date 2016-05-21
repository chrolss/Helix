#include "motors.h"

motors::motors(){
	initialize();
}

int motors::initialize(){
	this->pwmHandle = new pwm();
	pwmHandle->setFrequency(200000000);
	usleep(1000000); // give time to set frequency
	//pwmHandle->setQuadDutyCycle(stopDuty);
	usleep(1000000); // give time to start all ESC
	printf("Engines started\n");
	this->stopDuty[0] = 20.0;
	this->stopDuty[1] = 20.0;
	this->stopDuty[2] = 20.0;
	this->stopDuty[3] = 20.0;
	pwmHandle->setQuadDutyCycle(stopDuty);
	//pwmHandle->startQuadEngines();
	usleep(1000000);
	printf("Engines started\n");
	return 1;
}

int motors::closeMotors(){
	stopMotors();
	printf("Engines stopped\n");
	return 1;
}

double motors::mapper(double _preVal){
	return (29.0/100.0 * _preVal + 20);
}

int motors::setSpeed(double* _speeds){
	// &_speeds = [RF, RR, LR, LF]
	// speeds are 0 - 100% and will be mapped with mapper to 20 - 49
	this->internalDuty[0] = mapper(_speeds[0]);
	this->internalDuty[1] = mapper(_speeds[1]);
	this->internalDuty[2] = mapper(_speeds[2]);
	this->internalDuty[3] = mapper(_speeds[3]);
	pwmHandle->setQuadDutyCycle(internalDuty);
	return 1;
}

int motors::stopMotors(){
	pwmHandle->setQuadDutyCycle(stopDuty);
	return 1;
}
