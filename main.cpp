#include <iostream>
#include <stdio.h>
#include <chrono>
#include "src/motors.h"
#include "src/mpu9150.h"
#include "src/controller.h"
#include "src/com.h"

double sensorReadings[5];		//x-acc, y-acc, z-acc, pitch, roll
double comOutput[5];			//[x-ref, y-ref, z-ref, thrust, ...]
double motorVal[4];				//[RF, RR, LR, LF] pwm-values 0 - 100
double joyVal[4];				//[x-axis, y-axis, z-axis, throttle]
double references[3];		//[alpha_r, beta_r, gamma_r] - not determined yet

int main(){
	mpu9150* mpu = new mpu9150();
	motors* motor = new motors();
	controller* control = new controller();
	usleep(10000);
	std::cout << "before com" << std::endl;
	com* comHandle = new com();
	std::cout << "after com" << std::endl;
	//initialize the clock
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	double loopSleep;
	double loopTime;

	// Testing playground
	joyVal[0] = 500.0;
	joyVal[1] = 500.0;
	joyVal[2] = 0.0;
	joyVal[3] = 0.0;
	// End of playground

	for (int i = 0; i<1000; i++){
		auto start = std::chrono::high_resolution_clock::now();
		mpu->getSensorReadings(sensorReadings);
		control->getReferences(references, joyVal);
		control->getControlSignal(references, sensorReadings, motorVal);
		motor->setSpeed(motorVal);
		joyVal[3] = i;
		/*
		comHandle->sendAck();	//send to joystick that we want to read
		comHandle->readJoyVals(joyVal);
		mpu->getSensorReadings(sensorReadings);
		control->getReferences(references, joyVal);
		control->getControlSignal(references, sensorReadings, motorVal);
		motor->setSpeed(motorVal);
		*/
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
	comHandle->closeStream();
	return 1;
}
