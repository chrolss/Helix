#include "com.h"
#include <sstream>

com::com(){
	initialize();
}

void com::error(const char *msg)	//function for catching communication errors
{
    perror(msg);
    exit(1);
}


void com::initialize(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket in com initialize");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 9000;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	//bind socket to address
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //bind socket for communication
	error("ERROR on binding in com initialize");

	//listen for connection on sockfd, 5 is the amount of possible connections allowed
	printf("Listening for HelixControl\n");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
	if (newsockfd < 0)
		error("ERROR on accept host in com initialize");
	printf("HelixControl connected\n");
	connected = true;
	waitToSendMessage = true;

	//Start the communication loop after this, OR?
	// Call communication thread in main.cpp and from the
	// new thread we execute the communicationLoop()
}

void com::startCommunicationThread(){
	// Make sure this works and that everything is
	// declared here before entering the loop
	std::thread t1(&com::communicationLoop(), this);
	t1.detach();
}

void com::communicationLoop(){
	// the loop which handles the communication
	while (connected){
		if (waitToReadMessage){
			readFromHelixApp();
			waitToReadMessage = false;
			waitToSendMessage = true;
		}
		if (waitToSendMessage){
			sendToHelixApp();
			waitToReadMessage = true;
			waitToSendMessage = false;
		}
	}
	closeStream();
}

void com::readMsg(){
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket in com readMsg");
		int readVal = atoi(buffer);
		printf("val: %f \n",readVal/42007.0);
}

void com::setOutputData(double *_sensorReadings, double *_motorsVals){
	outputs[0] = _sensorReadings[3]*RADTODEG;
	outputs[1] = _sensorReadings[4]*RADTODEG;
	outputs[2] = _motorsVals[0];
	outputs[3] = _motorsVals[1];
	outputs[4] = _motorsVals[2];
	outputs[5] = _motorsVals[3];
}

void com::sendToHelixApp(){
	ostr.str("");
	for (int i = 0; i<6; i++){
		ostr << outputs[i] << ":";
	}
	sendMessage = ostr.str();
	n = write(newsockfd, sendMessage.c_str(), sendMessage.length());
	if (n<0){
		printf("Error sendToHelixApp\n");
	}
}

void com::readFromHelixApp(){
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n<0){
		printf("error in readFromHelixApp\n");
	}
	sscanf(buffer, "%lf:%lf:%lf:%lf:%lf", &inputs[0], &inputs[1], &inputs[2], &inputs[3], &inputs[4]);

}

void com::getInputData(double *_joyVal){
	_joyVal[0] = inputs[0];
	_joyVal[1] = inputs[1];
	_joyVal[2] = inputs[2];
	_joyVal[3] = inputs[3];
	_joyVal[4] = inputs[4];
}

void com::readHelixApp(double *_joyVal, double *_sensorReadings, double *_motorsVals){
	// function to use together with the iPad app HelixControl
	bzero(buffer,256);
	// like i2c, send that we want to read
	// WRITE PART OF THE CODE
	ostr.str("");
	// ostr = "pitch:roll:RF:RR:LR:LF"

	ostr << _sensorReadings[3]*RADTODEG << ":" << _sensorReadings[4]*RADTODEG << ":" << _motorsVals[0] << ":" << _motorsVals[1] << ":" << _motorsVals[2] << ":" << _motorsVals[3];
	sendMessage = ostr.str();

	//sendMessage = "1:2:3:4:5:6";
	//sendAck();
	int n = write(newsockfd,sendMessage.c_str(),sendMessage.length());
	// READ PART OF THE CODE
	n = read(newsockfd,buffer,255);

	//New CODE to be tested
	sscanf(buffer, "%lf:%lf:%lf:%lf:%lf", &_joyVal[0], &_joyVal[1], &_joyVal[2], &_joyVal[3], &_joyVal[4]);

}

void com::readJoyVals(double *_joyVal){
	//special function that reads only the joystick values from the QuadCenter program
	int readVal[3];
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket in com readJoyVals");
	//printf("Here is the message: %s \n",buffer);
	//printf("Here is the second msg: %f \n", atof(buffer[1]));
	//break communication if client sends "quit"
	std::string str(buffer);
	std::istringstream iss(str);
	std::string sub;
	for (int i = 0;i<4;i++){
		// i<x where x is number of actions read, (buttons and axes)
		iss >> sub;
		//readVal[i] = std::stoi (sub);
		_joyVal[i] = std::stoi (sub);
	}
}

void com::sendAck(){
	n = write(newsockfd,"send",4);
	if (n<0) error("error sending ack bit in com sendAck\n");
}

void com::closeStream(){
    //close the socket and remove the connection
    close(newsockfd);
    close(sockfd);
}
