#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
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
private:
	void initialize();
	void error(const char *msg);
	int sockfd, newsockfd, portno;				//bit addresses
	socklen_t clilen;
	char buffer[256];							//messages buffer
	int msgBuffer[256];
	struct sockaddr_in serv_addr, cli_addr;	//communcation structs
	int n;		//communcation handle
	int count;
	bool connected;
	bool waitToReadMessage;
	bool waitToSendMessage;
	std::string sendMessage;
	std::ostringstream ostr;
};
