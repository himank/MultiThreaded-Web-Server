/*
 * myserver.h
 *
 *  Created on: 23-Sep-2012
 *      Author: himank
 */

#ifndef MYSERVER_H_
#define MYSERVER_H_

#ifndef MAXCONNECTION
#define MAXCONNECTION 10
#endif
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class RunServer
{
public:
		struct addrinfo inValue, *serverInfo, *validInfo;
		struct sockaddr_storage clientAddr;
		int acceptId, address,yes;
		char ip[INET6_ADDRSTRLEN];
		char ip1[INET6_ADDRSTRLEN];
		socklen_t addrlen;
		//struct clientInfo *rclientData;
		RunServer();
		void accept_connection();
		void *get_ip_address(sockaddr *s);
		u_int16_t get_port_number(struct sockaddr *s);

};

#endif /* MYSERVER_H_ */
