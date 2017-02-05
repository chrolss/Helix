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
}

void com::readMsg(){
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket in com readMsg");
		//printf("Here is the message: %s \n",buffer);
		//printf("Here is the second msg: %f \n", atof(buffer[1]));
		//break communication if client sends "quit"
		//std::string str(buffer);
		//int readVal = std::stoi (str);
		int readVal = atoi(buffer);
		printf("val: %f \n",readVal/42007.0);
}

void decodeMessage(char _msg[],double *_joyVal){
	//only used as reference to readHelixApp
  printf("Here is the msg from decoder: %s \n", _msg);
  sscanf(_msg, "%lf:%lf:%lf:%lf", &_joyVal[0], &_joyVal[1], &_joyVal[2], &_joyVal[3]);
  for (int i = 0; i<5; i++){
    std::cout << joyVal[i] << std::endl;
  }
}

void com::readHelixApp(double *_joyVal){
	// function to use together with the iPad app HelixControl
	bzero(buffer,256);
	// like i2c, send that we want to read
	sendAck();
	n = read(newsockfd,buffer,255);

	//New CODE to be tested
	sscanf(buffer, "%lf:%lf:%lf:%lf", &_joyVal[0], &_joyVal[1], &_joyVal[2], &_joyVal[3]));
	for (int i = 0; i<5; i++){
		std::cout << joyVal[i] << std::endl;
	}

	/*
	OLD WORKING CODE (kind of at least)
	if (n < 0) error("ERROR reading from socket in com readHelixApp\n");
	double val = atof(buffer);
	_joyVal[3] = val;
	printf("received value: %s, converted:  %f \n", buffer, val);
	*/
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
	//int readVal = std::stoi (str);
	/*
	readVal[i] = atoi(buffer);
	double endVal0 = readVal[0]/42007.0;
	double endVal1 = readVal[1]/42007.0;
	double throttle = readVal[2];
	*/
	//printf("val: %d, val: %d, val: %d\n", readVal[0], readVal[1], readVal[2]);

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
