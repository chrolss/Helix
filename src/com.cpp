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
	printf("Here is the message: %s \n",buffer);
	//printf("Here is the second msg: %f \n", atof(buffer[1]));
	//break communication if client sends "quit"
	std::string str(buffer);
	std::string subst = str.substr(0,4);
	if (subst == "quit"){
		printf("The client has quit the conversation \n");
		n = write(newsockfd, "Closing the connection",22);
	 }

	 //Write back to client
	else{
		n = write(newsockfd,"I got your message",18);
		if (n < 0) error("ERROR writing to socket");
	}
}


void com::closeStream(){
    //close the socket and remove the connection
    close(newsockfd);
    close(sockfd);
}
