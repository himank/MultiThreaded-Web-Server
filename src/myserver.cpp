//============================================================================
// Name        : myserver.cpp
// Author      : Himank Chaudhary
// Version     :
// Copyright   : Your copyright notice
// Description : Server Implementation is here in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../src/myserver.h"
#include "../src/parse.h"
#include "../src/myhttpd.h"

using namespace std;

// Set Default values
RunServer::RunServer()
{
	memset(&inValue, 0, sizeof(inValue));
	inValue.ai_family = AF_UNSPEC;
	inValue.ai_socktype = SOCK_STREAM;
	inValue.ai_flags = AI_PASSIVE;
	yes=1;
}

// Return Port Number
u_int16_t RunServer::get_port_number(struct sockaddr *s)
{
	if(s->sa_family == AF_INET)
		return (((struct sockaddr_in  *)s)->sin_port);
	else
		return (((struct sockaddr_in6 *)s)->sin6_port);
}

//Return Ip address
void * RunServer::get_ip_address(sockaddr *s)
{
	if(s->sa_family == AF_INET)
		return &((sockaddr_in *)s)->sin_addr;
	else
		return &((sockaddr_in6 *)s)->sin6_addr;
}

// This function will create the socket for all the incoming client connections & update the client structure and pass
// this structure to Parse class
void RunServer::accept_connection()
{
	//struct sockaddr_in serverInfo;

	if (getaddrinfo(NULL, port.c_str(), &inValue, &serverInfo) != 0)
		    perror("Get Address:");

	for(validInfo = serverInfo; validInfo != NULL; validInfo = validInfo->ai_next) {
		if((sockId = (socket(validInfo->ai_family, validInfo->ai_socktype,0))) == -1)
				perror("Socket:");
		
		addrlen = sizeof(serverInfo);
		
		if (setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1){
			perror("setsockopt");
		    break;
		}
		
		if(bind(sockId,validInfo->ai_addr, validInfo->ai_addrlen) == -1)
				perror("Bind:");
		
		break;
	}

	// successfully done with bind
	
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)validInfo->ai_addr;
	void *addr;
	addr = &(ipv4->sin_addr);
	inet_ntop(validInfo->ai_family,addr, ip, sizeof(ip));
	freeaddrinfo(serverInfo);
	if(listen(sockId, MAXCONNECTION) == -1)
		perror("Listen:");

	while(true) {		// Main thread will listen continously 
		address = sizeof(clientAddr);
		
		if((acceptId = accept(sockId,(struct sockaddr*)&clientAddr,(socklen_t *)&address)) == -1)
				perror("Accept:");
		
		inet_ntop(clientAddr.ss_family,get_ip_address((struct sockaddr *)&clientAddr),ip1, sizeof(ip1));
		u_int16_t clientport = get_port_number((struct sockaddr *)&(validInfo));
		time_t tim=time(NULL);
		tm *now=gmtime(&tim);
		char currtime[50];
		
		if (strftime(currtime, 50,"%x:%X", now) == 0)
				perror("Date Error");
		
		string requesttime(currtime);
		clientIdentity cid;
		cid.acceptId = acceptId;
		string s(ip1);
		cid.ip=s;
		cid.portno = clientport;
		cid.requesttime = requesttime;
		P->parseRequest(cid);
	}
}