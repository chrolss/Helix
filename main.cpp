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
double joyVal[5];				//[x-axis, y-axis, z-axis, throttle]
double references[3];		//[alpha_r, beta_r, gamma_r] - not determined yet
// motors on is now in joyVal[4] ->  1 = motorsOn , 0 = motorsOff

int main(){
	mpu9150* mpu = new mpu9150();
	motors* motor = new motors();
	controller* control = new controller();
	usleep(10000);
	com* comHandle = new com();
	//initialize the clock
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	double loopSleep;
	double loopTime;

	//NEW CODE
	comHandle->startCommunicationThread();

	//for (int i = 0; i<100000; i++){
	while (comHandle->getConnectedStats()){
		auto start = std::chrono::high_resolution_clock::now();
		mpu->getSensorReadings(sensorReadings);
		//comHandle->readHelixApp(joyVal, sensorReadings, motorVal);

		//NEW CODE
		// do I need the if statements?
			//if (comHandle->waitToRead())
			comHandle->setOutputData(sensorReadings, motorVal);
			//if (comHandle->waitToSend())
			comHandle->getInputData(joyVal);
		//

		control->getReferences(references, joyVal);
		control->getControlSignal(references, sensorReadings, motorVal);

		//Check if enginges should be on or off
		if (joyVal[4]){
			motor->setSpeed(motorVal);

			//printf("RF: %f, RR: %f, LR: %f, LF: %f\n", motorVal[0], motorVal[1], motorVal[2], motorVal[3]);
		} else {
			motor->stopMotors();
			printf("Enginges off\n");
		}

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
