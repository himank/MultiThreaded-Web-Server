// Copyright 2022 @HimankChaudhary
//
// Author      : Himank Chaudhary
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
