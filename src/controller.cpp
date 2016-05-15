#include "controller.h"

controller::controller(){
	initialize();
}

void controller::initialize(){
	this->dt = 0.025; // ???
	this->ePitch[2] = 0.0;
	this->eRoll[2] = 0.0;
	this->eYaw[2] = 0.0;
	readParameters("src/pidParams.txt");
	printf("p1: %f, p2: %f, p3: %f\n", parameters[0], parameters[3], parameters[6]);

}

void controller::readParameters(std::string _paramsFileName){
	std::fstream paramsFile(_paramsFileName);
	std::string line;
	int i = 0;
	if (paramsFile.is_open()) {
		while (getline(paramsFile, line)) {
			parameters[i] = atof(line.c_str());
			i++;
		}
	}
	else {
		printf("Error reading pidParams.txt");
	}

	paramsFile.close();
	printf("Got Parameters\n");
}


void controller::getReferences(double *_refs, double *_joyVal){
	// joyVal = [x-axis, y-axis, throttle], max = 32767, min -32767
	// refs should be in rad
	// _refs = [alpha, beta, gamma, throttle], maxAngle = +-0.79 rad, throttle = [0 - 100]
	_refs[0] = _joyVal[0]*0.00002411;
	_refs[1] = _joyVal[1]*0.00002411;
	_refs[2] = 0.0; //_joyVal[2]*0.00002411;
	_refs[3] = _joyVal[2]*100.0/32767.0;
}

double controller::signalLimiter(double _signal){
	// makes sure the sent signal is between 0 - 100
	if (_signal > MAX_SIGNAL){
		return MAX_SIGNAL;
	}
	else if (_signal < MIN_SIGNAL){
		return MIN_SIGNAL;
	}
	else{
		return _signal;
	}
}

void controller::getControlSignal(double *_refs, double *_sensorReadings, double *_controlSignals){
	// sensorReadings[] = x-acc, y-acc, z-acc, pitch, roll, yaw (not added)
	// refs[] = pitch, roll, yaw
	// TODO: anti-windup
	//pitch
	this->ePitch[0] = _refs[0] - _sensorReadings[3]; // + trim in future
	this->ePitch[2] = ePitch[2] + ePitch[0]*dt;
	this->MpT = parameters[0]*ePitch[0] + parameters[1]*ePitch[2] + parameters[2]*(ePitch[0] - ePitch[1])/dt;
	this->ePitch[1] = ePitch[0];
	//roll
	this->eRoll[0] = _refs[1] - _sensorReadings[4]; // + trim in future
	this->eRoll[2] = eRoll[2] + eRoll[0]*dt;
	this->MrT = parameters[3]*eRoll[0] + parameters[4]*eRoll[2] + parameters[5]*(eRoll[0] - eRoll[1])/dt;
	this->eRoll[1] = eRoll[0];
	//yaw
	this->eYaw[0] = _refs[2] - _sensorReadings[5];// + trim in future
	this->eYaw[2] = eYaw[2] + eYaw[0]*dt;
	this->MyT = parameters[6]*eYaw[0] + parameters[7]*eYaw[2] + parameters[8]*(eYaw[0] - eYaw[1])/dt;
	this->eYaw[1] = eYaw[0];
	//calculate the actual control torques
	Ma = MrT*cos45 - MpT*sin45;
	Mb = MrT*cos45 + MpT*sin45;
	Mg = -MyT;
	F = 0.045; //4*THRUST_CONST*0.30*0.30*10000.0; // ??
	//return the control signals
	//printf("Mb: %f, Ma: %f, Mg: %f\n", Ma, Mb, Mg);
	_controlSignals[0] = signalLimiter(0.25*(F*C1 - Mb*C2 + Mg*C3));	//RF
	_controlSignals[1] = signalLimiter(0.25*(F*C1 - Ma*C2 - Mg*C3));	//RR
	_controlSignals[2] = signalLimiter(0.25*(F*C1 + Mb*C2 + Mg*C3));	//LR
	_controlSignals[3] = signalLimiter(0.25*(F*C1 + Ma*C2 - Mg*C3));	//LF
}
