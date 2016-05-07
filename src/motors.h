#include <iostream>
#include "pwm.h"

class motors{
public:
	motors();
	int setSpeed(double* _speeds);
	int stopMotors();
	int closeMotors();
private:
	int initialize();
	double mapper(double _preVal);
	pwm* pwmHandle;
	double stopDuty[4];
	double internalDuty[4];
};
