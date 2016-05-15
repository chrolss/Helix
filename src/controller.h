#include <stdio.h>
#include <string>
#include <fstream>

#define cos45 0.707106
#define sin45 0.707106
#define C1 3333.33
#define C2 277
#define C3 1e7
#define THRUST_CONST 0.0003
#define MAX_SIGNAL 50.0
#define MIN_SIGNAL 0.0

class controller{
public:
	controller();
	void getControlSignal(double *_refs, double *_sensorReadings, double *_controlSignals);
	void getReferences(double *_refs, double *_joyVal);
private:
	void initialize();
	void readParameters(std::string _paramsFileName);	//reads PID params from file
	void writeParameters();	//writes PID params to file
	double signalLimiter(double _signal);
	double eRoll[3];	//0 = current, 1 = previous, 2 = integrated
	double ePitch[3];
	double eYaw[3];
	double MrT;			//temp roll torque
	double MpT;			//temp pitch torque
	double MyT;			//temp yaw torque
	double Ma;			//torque around alpha axis
	double Mb;			//torque around beta axis
	double Mg;			//torque around gravity axis
	double F;			//thrust force
	double dt;
	double parameters[9];	//0-2 pitchPID, 3-5 rollPID, 6-8 yawPID
};
