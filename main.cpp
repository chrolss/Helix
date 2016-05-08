#include <iostream>
#include <stdio.h>
#include <chrono>
#include "src/motors.h"
#include "src/mpu9150.h"
#include "src/controller.h"

double sensorReadings[5];		//x-acc, y-acc, z-acc, pitch, roll
double comOutput[5];			//[x-ref, y-ref, z-ref, thrust, ...]
double motorVal[4];				//[RF, RR, LR, LF] pwm-values 0 - 100
double references[3];

int main(){
	mpu9150* mpu = new mpu9150();
	motors* motor = new motors();
	controller* control = new controller();
	usleep(10000);
	//initialize the clock
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	double loopSleep;
	double loopTime;

	for (int i = 0; i<150; i++){
		auto start = std::chrono::high_resolution_clock::now();
		mpu->getSensorReadings(sensorReadings);
		control->getReferences(references);
		control->getControlSignal(references, sensorReadings, motorVal);
		motor->setSpeed(motorVal);
		printf("RF: %f, RR: %f, LR: %f, LF: %f\n", motorVal[0], motorVal[1], motorVal[2], motorVal[3]);
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
		loopSleep = 10000 - (int)duration;
		//printf("Duration: %d\n", (int)duration);
		if (loopSleep>0){
			usleep(loopSleep);
		}
		auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();

		loopTime = double(1000000)/(duration2);
		std::cout << "Running at: " << loopTime << "Hz" << std::endl;

	}

	motor->closeMotors();
	return 1;
}
