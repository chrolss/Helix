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
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 9000;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	//bind socket to address
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) //bind socket for communication
	error("ERROR on binding");

	//listen for connection on sockfd, 5 is the amount of possible connections allowed
	printf("Listening for QuadCenter\n");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	printf("QuadCenter connected\n");
}

void com::readMsg(){
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		//printf("Here is the message: %s \n",buffer);
		//printf("Here is the second msg: %f \n", atof(buffer[1]));
		//break communication if client sends "quit"
		//std::string str(buffer);
		//int readVal = std::stoi (str);
		int readVal = atoi(buffer);
		printf("val: %f \n",readVal/42007.0);
}

void com::readJoyVals(){
	int readVal[3];
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	//printf("Here is the message: %s \n",buffer);
	//printf("Here is the second msg: %f \n", atof(buffer[1]));
	//break communication if client sends "quit"
	std::string str(buffer);
	std::istringstream iss(str);
	std::string sub;
	for (int i = 0;i<3;i++){
		iss >> sub;
		readVal[i] = std::stoi (sub);
	}
	//int readVal = std::stoi (str);
	/*
	readVal[i] = atoi(buffer);
	double endVal0 = readVal[0]/42007.0;
	double endVal1 = readVal[1]/42007.0;
	double throttle = readVal[2];
	*/
	printf("val: %d, val: %d, val: %d\n", readVal[0], readVal[1], readVal[2]);

}

void com::sendAck(){
	n = write(newsockfd,"send",4);
	if (n<0) error("error sendign ack\n");
}

void com::closeStream(){
    //close the socket and remove the connection
    close(newsockfd);
    close(sockfd);
}
