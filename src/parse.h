/*
 * parse.h
 *
 *  Created on: 25-Sep-2012
 *      Author: himank
 */

//#include<queue>
#ifndef PARSE_H_
#define PARSE_H_
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<iostream>
#include <string>
#include<fstream>
#include <list>
#include <vector>
using namespace::std;
//enum RequestMethod {GET,HEAD,OTHER};

//typedef struct clientInfo;
//typedef struct clientIdentity;
struct clientIdentity
{

	int acceptId;
	string ip;
	int portno;
	string requesttime;
};
struct clientInfo
{
	string r_method;
	string r_type;
	string r_version;
	string r_firstline;
	//char filename[150];
	string r_filename;
	string r_time;
	string r_servetime;
	int r_acceptid;
	string r_ip;
	u_int16_t r_portno;
	int r_filesize;
	bool status_file;
	string r_ctype;
	bool rootcheck;
	int status_code;

};

class Parse
{
public:
		//struct clientInfo cInfo;



		//char *parseWord[4],

		list<clientInfo> clientlist;
		list<clientInfo> requestlist;
		//vector<string> dirlist;
		//Parse();
		//Parse(clientIdentity *);
		//static void * parseRequest_helper(void *c);
		bool fileExists(char *filename);
		void parseRequest(clientIdentity);
		void readyQueue(clientInfo );
		void checkRequest(clientInfo);
		void fileRequestedData();
		//void schedulerThread();
		void popRequest();
		static void *popRequest_helper(void *);
		void serveRequest();
		static void *serveRequest_helper(void *);
		void changeFolder(clientInfo);


};
#endif /* PARSE_H_ */
