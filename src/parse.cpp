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

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>
#include <time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "../src/myhttpd.h"
#include "../src/parse.h"
#include "../src/senddata.h"
#include <dirent.h>
#include<algorithm>


using namespace std;

// This function have the current client structure, this function will parse the request and fill the fields
void  Parse::parseRequest(clientIdentity c_id)
{
	int i=1;
	int recvbytes=0;
	char buffer[1024];
	clientInfo cInfo;

	char *p,*pbuffer;

	fcntl( c_id.acceptId, O_NONBLOCK, 0 );
	
	if((recvbytes = (recv(c_id.acceptId, buffer, sizeof(buffer),0))) == -1)
		perror("Receive:");
	buffer[recvbytes] = '\0';
	string tofetchfline(buffer);
	
	int current = 0;
	int next = tofetchfline.find_first_of("\r\n", current );
	if(next < 0) {
		write(cInfo.r_acceptid,"Error: Bad Request, Retry",25);
		close (cInfo.r_acceptid);
	} else {
		cInfo.r_firstline = tofetchfline.substr( current, next - current);
		string parseWord[3];
		pbuffer = new char[cInfo.r_firstline.size() + 1];
		std::copy(cInfo.r_firstline.begin(), cInfo.r_firstline.end(), pbuffer);
		pbuffer[cInfo.r_firstline.size()] = '\0';

		p=strtok (pbuffer," /");
		parseWord[0].assign(p);
		while (i < 3) {
			  p=strtok(NULL," ");
			  parseWord[i].assign(p);
			  i++;
		}

		cInfo.r_acceptid = c_id.acceptId;
		cInfo.r_portno = c_id.portno;
		cInfo.r_ip = c_id.ip;
		cInfo.r_time = c_id.requesttime;
		cInfo.r_type = parseWord[0];
		cInfo.r_filename = parseWord[1];
		
		int ii = cInfo.r_filename.find_first_of("/",0);
		int jj = cInfo.r_filename.find_first_of("/",ii+1);
		if(jj >= 0)
				cInfo.rootcheck = false;
		else
				cInfo.rootcheck = true;
		cInfo.r_filename = rootdir + cInfo.r_filename;
		cInfo.r_method = parseWord[2];
		
		delete [] pbuffer;
		pbuffer = NULL;
		changeFolder(cInfo);
	}
}

// If tilt is present in the client request then change the folder
void Parse::changeFolder(clientInfo c)
{
	int next = c.r_filename.find_first_of("~",0);
	int size = c.r_filename.size();
	if(next > 0 && next < size) {
		int pos = c.r_filename.find_first_of("/",next);
		string requestfor = c.r_filename.substr(next+1,pos -(next+1));
		string restString = c.r_filename.substr(pos,c.r_filename.size()-pos);
		c.r_filename.erase(next,c.r_filename.size());
		c.r_filename = c.r_filename + requestfor +"/myhttpd" +restString;
	}

	checkRequest(c);
}

// if file requested exists or not
bool Parse::fileExists(char *filename)						
{
	struct stat filenamecheck;
    if (stat(filename, &filenamecheck) != -1)
       	   	return true;
    
	return false;
}

// Check type of request
void Parse::checkRequest(clientInfo cInfo)
{

	int pos = cInfo.r_filename.find_last_of("/");
	int next = cInfo.r_filename.find_first_of(".",pos);
	if(next > 0) {
		char * fname = new char[cInfo.r_filename.size() + 1];
		std::copy(cInfo.r_filename.begin(), cInfo.r_filename.end(), fname);
		fname[cInfo.r_filename.size()] = '\0';
		ifstream::pos_type size = 0;

		if(fileExists(fname) && cInfo.r_type == "GET") {
			ifstream file;
			file.open(fname);
			if (file.is_open()) {
				file.seekg (0, ios::end);
				size = file.tellg();
			}
			cInfo.status_file = true;
			cInfo.r_filesize = (int)size;
			file.close();
			delete [] fname;
			fname = NULL;
			if(r_daemon) {
				SendData S;
				S.sendData(cInfo);
			} else {
				readyQueue(cInfo);
			}
		} else if(fileExists(fname) && cInfo.r_type == "HEAD") {
			cInfo.status_file = true;
			cInfo.r_filesize = 0;
			delete [] fname;
			fname = NULL;
			if(r_daemon) {
				SendData S;
				S.sendData(cInfo);
			} else {
				readyQueue(cInfo);
			}
		} else {
			delete [] fname;
			fname = NULL;
			cInfo.r_filesize = 0;
			cInfo.status_file = false;
			if(r_daemon) {
				SendData S;
				S.sendData(cInfo);
			} else {
				readyQueue(cInfo);
			}
		}
	} else {
		cInfo.r_filesize = 0;
		cInfo.status_file = false;
		if(r_daemon) {
			SendData S;
			S.sendData(cInfo);
		} else {
			readyQueue(cInfo);
		}
	}
}


// Sort the request based on file size
bool sortRequest(const clientInfo& lhs, const clientInfo& rhs)
{
  return lhs.r_filesize < rhs.r_filesize;
}

// Here main thread will put the request in the queue
void Parse::readyQueue(clientInfo cInfo)
{
	pthread_mutex_lock(&rqueue_lock);
	clientlist.push_back(cInfo);
	pthread_cond_signal(&rqueue_cond);
	pthread_mutex_unlock(&rqueue_lock);
}

// In this scheduler thread will fetch the request from the queue based on the scheduling policies
void Parse::popRequest()
{
	sleep(r_time);
	while(1) {
		clientInfo c;
		transform(scheduling.begin(), scheduling.end(),scheduling.begin(),::toupper);
		if(scheduling =="SJF") {
			pthread_mutex_lock(&rqueue_lock);
			
			while(clientlist.empty())
				pthread_cond_wait(&rqueue_cond, &rqueue_lock);
			
			clientlist.sort(sortRequest);
			c = clientlist.front();
			clientlist.pop_front();
			pthread_mutex_unlock(&rqueue_lock);
		} else {
			pthread_mutex_lock(&rqueue_lock);
			while(clientlist.empty())
				pthread_cond_wait(&rqueue_cond, &rqueue_lock);
			c = clientlist.front();
			clientlist.pop_front();
			pthread_mutex_unlock(&rqueue_lock);
		}
		
		pthread_mutex_lock(&print_lock);
		requestlist.push_back(c);
		pthread_cond_signal(&print_cond);
		pthread_mutex_unlock(&print_lock);
	}
}

void *Parse::popRequest_helper(void *c)
{
	//Parse *P =(Parse *)c;
	//P->popRequest();
	((Parse *)c)->popRequest();
	//delete P;
	return NULL;
}

void *Parse::serveRequest_helper(void *c)
{
	Parse *P1 =(Parse *)c;
	P1->serveRequest();
	return NULL;
}

// In this function contionsly threads are acting to serve the client request.
void Parse::serveRequest()
{
	pthread_detach(pthread_self());
	while(1) {
		pthread_mutex_lock(&print_lock);
		
		while(requestlist.empty())
				pthread_cond_wait(&print_cond, &print_lock);
		
		SendData S;
		clientInfo c;
		c = requestlist.front();
		requestlist.pop_front();
		time_t tim=time(NULL);
		tm *now=gmtime(&tim);
		char currtime[50];
		if (strftime(currtime, 50,"%x:%X", now) == 0)
				perror("Date Panga");
		string servetime(currtime);
		c.r_servetime = servetime;
		pthread_mutex_unlock(&print_lock);
		S.sendData(c);
	}
}
