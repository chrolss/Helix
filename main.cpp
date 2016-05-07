#include <iostream>
#include <stdio.h>
#include "src/motors.h"
#include "src/mpu9150.h"

double sensorReadings[5];	//pitch, roll, yaw
double comOutput[5];			//[x-ref, y-ref, z-ref, thrust, ...]
double motorVal[4];

int main(){
	mpu9150* mpu = new mpu9150();
	motors* motor = new motors();
	usleep(1000000);
	for (int i = 0; i<200; i++){
		mpu->getSensorReadings(sensorReadings);
		printf("x-acc: %f, y-acc: %f, z-acc: %f, pitch: %f, roll: %f\n", sensorReadings[0], sensorReadings[1],sensorReadings[2],sensorReadings[3],sensorReadings[4]);
		for (int j = 0; j<4; j++){
			motorVal[j] = abs(sensorReadings[3])+10.0;
		}
		motor->setSpeed(motorVal);
		//usleep(100000);
	}
	motor->closeMotors();
	return 1;
}
