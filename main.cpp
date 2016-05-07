#include <iostream>
#include <stdio.h>
#include "src/motors.h"
#include "src/mpu9150.h"
#include "src/controller.h"

double sensorReadings[5];	//x-acc, y-acc, z-acc, pitch, roll
double comOutput[5];			//[x-ref, y-ref, z-ref, thrust, ...]
double motorVal[4];
double references[3];

int main(){
	mpu9150* mpu = new mpu9150();
	motors* motor = new motors();
	controller* control = new controller();
	usleep(10000);
	for (int i = 0; i<300; i++){
		mpu->getSensorReadings(sensorReadings);
		control->getReferences(references);
		control->getControlSignal(references, sensorReadings, motorVal);
		printf("RF: %f, LF: %f, LR: %f, RR: %f\n", motorVal[0], motorVal[1], motorVal[2], motorVal[3]);
		usleep(100000);
	}
	/*
	for (int i = 0; i<300; i++){
		mpu->getSensorReadings(sensorReadings);
		printf("pitch: %f, roll: %f\n", sensorReadings[3], sensorReadings[4]);
		usleep(100000);
	}
	*/
	/*
	for (int i = 0; i<200; i++){
		mpu->getSensorReadings(sensorReadings);
		printf("x-acc: %f, y-acc: %f, z-acc: %f, pitch: %f, roll: %f\n", sensorReadings[0], sensorReadings[1],sensorReadings[2],sensorReadings[3],sensorReadings[4]);
		for (int j = 0; j<4; j++){
			motorVal[j] = abs(sensorReadings[3])+10.0;
		}
		motor->setSpeed(motorVal);
		//usleep(100000);
	}
	*/
	motor->closeMotors();
	return 1;
}
