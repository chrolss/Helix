#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>

#define RADTODEG 52.295

class com{
public:
	com();
	void readMsg();
	void closeStream();
	void sendAck();
	void readJoyVals(double *_joyVal);
	void readHelixApp(double *_joyVal, double *_sensorReadings, double *_motorsVals);
	void communicationLoop();
	void startCommunicationThread();
	void setOutputData(double *_sensorReadings, double *_motorsVals);
	void getInputData(double *_joyVal);
	void sendToHelixApp();
	void readFromHelixApp();
	bool waitToRead(){return waitToReadMessage;}
	bool waitToSend(){return waitToSendMessage;}
	bool getConnectedStats(){return connected;}
private:
	void initialize();
	void error(const char *msg);
	int sockfd, newsockfd, portno;				//bit addresses
	socklen_t clilen;
	char buffer[256];							//messages buffer
	int msgBuffer[256];
	struct sockaddr_in serv_addr, cli_addr;	//communication structs
	int n;		//communication handle
	int count;
	bool connected;
	bool waitToReadMessage;
	bool waitToSendMessage;
	double outputs[6]; //these values are sent to the helix app
	double inputs[5]; //these values were read from the helix app
	std::string sendMessage;
	std::ostringstream ostr;
	std::string connectedString;
	std::ostringstream out0;
	std::ostringstream out1;
	std::ostringstream out2;
	std::ostringstream out3;
	std::ostringstream out4;
	std::ostringstream out5;

};
