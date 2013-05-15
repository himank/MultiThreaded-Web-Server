//============================================================================
// Name        : send.cpp
// Author      : Himank Chaudhary
// Version     :
// Copyright   : Your copyright notice
// Description : Serving of client data is here
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>
#include <time.h>
#include <pthread.h>
#include <sstream>
#include <dirent.h>
#include <vector>
#include<algorithm>
#include "../src/parse.h"
#include "../src/senddata.h"
#include"../src/myhttpd.h"
using namespace std;

void SendData::sendData(clientInfo c)				// Sending data
{

	if(c.status_file ==true && c.r_type =="GET")
	{
		char  currtime[50];
		char lastmodify[50];
		struct stat buf;
		time_t tim=time(NULL);
		tm *now=gmtime(&tim);

		if (strftime(currtime, 50,"%c", now) == 0)
				perror("Date Error");
		char * fname = new char[c.r_filename.size() + 1];
		std::copy(c.r_filename.begin(), c.r_filename.end(), fname);
		fname[c.r_filename.size()] = '\0';
		stat(fname,&buf);
		strcpy(lastmodify,ctime(&buf.st_mtime));
		delete[] fname;
		fname = NULL;
		int pos = c.r_filename.find_last_of(".");
		string c_type = c.r_filename.substr(pos+1,c.r_filename.size());
		
		// Type of file requested
		if (c_type == "txt" || c_type == "html" || c_type == "htm")
				c.r_ctype ="text/html";
		else if(c_type == "gif" || c_type == "jpeg")
				c.r_ctype = "image/"+c_type;
		else
				c.r_ctype = " ";

		std::stringstream ss;
		ss << c.r_filesize;
		string filesize = ss.str();
		string header = c.r_type + " "+ c.r_method +" 200 OK\r\nDate: ";
		string time(currtime);
		header = header + time;
		header = header +"\r\n" +"Server: myhttpd 1.0\r\n" + "Last-Modified:";
		string lmodify(lastmodify);
		header = header + lmodify ;//+"\r\n";
		header = header + "Content-Type:" + c.r_ctype +"\r\n" + "Content-Length:" + filesize +"\r\n\r\n";

		if (send(c.r_acceptid, header.c_str(), strlen(header.c_str()), 0) == -1)
									       perror("send");


		ifstream file;
		char *readblock;
		size_t size;
		file.open(c.r_filename.c_str());
		if (file.is_open())
		{
			string read;
			file.seekg (0, ios::end);
			size = file.tellg();
			readblock = new char [size];
			file.seekg (0, ios::beg);
			file.read(readblock, size);
		}
		else
			cout<<"Never went Inside"<<endl;
		if (send(c.r_acceptid, readblock, size, 0) == -1)
	       perror("send");
		file.close();
		c.status_code = 200;
		delete [] readblock;

	}
	else if(c.status_file ==true && c.r_type =="HEAD")
	{
				char  currtime[50];
				char lastmodify[50];
				struct stat buf;
				//bool logging = true;
				time_t tim=time(NULL);
				tm *now=gmtime(&tim);

				if (strftime(currtime, 50,"%c", now) == 0)
						perror("Date Panga");

				char * fname = new char[c.r_filename.size() + 1];
				std::copy(c.r_filename.begin(), c.r_filename.end(), fname);
				fname[c.r_filename.size()] = '\0';
				stat(fname,&buf);
				strcpy(lastmodify,ctime(&buf.st_mtime));
				delete[] fname;
				fname = NULL;

				int pos = c.r_filename.find_last_of(".");
				string c_type = c.r_filename.substr(pos+1,c.r_filename.size());
				if (c_type == "txt" || c_type == "html" || c_type == "htm")
						c.r_ctype ="text/html";
				else if(c_type == "gif" || c_type == "jpeg")
						c.r_ctype = "image/"+c_type;
				else
						c.r_ctype = " ";

				std::stringstream ss;
				ss << c.r_filesize;
				string filesize = ss.str();
				string header = c.r_type + " "+ c.r_method +" 200 OK\r\nDate: ";
				string time(currtime);
				header = header + time;
				header = header +"\r\n" +"Server: myhttpd 1.0\r\n" + "Last-Modified:";
				string lmodify(lastmodify);
				header = header + lmodify ;//+"\r\n";
				header = header + "Content-Type:" + c.r_ctype +"\r\n" + "Content-Length:" + filesize +"\r\n\r\n";

				if (send(c.r_acceptid, header.c_str(), strlen(header.c_str()), 0) == -1)
											       perror("send");

				c.status_code = 200;



	}
	else
		{
			if (c.rootcheck == true)
			{
				write(c.r_acceptid,"Error 404: File Not Found",25);
				c.status_code = 404;
			}
			else
				c.status_code = 404;
				listingDir(c);


		}

	if(logging) // If request need to be logged
	{
		generatingLog(c);
	}
	if(r_daemon) // If daemon is set
	{
		displaylog(c);
	}
	close(c.r_acceptid);


}


void SendData::generatingLog(clientInfo c)
{
	ofstream logfile;
	string file = l_file;
	logfile.open(file.c_str(), std::ios::app);
	if( !logfile )
	{ 						// file couldn't be opened
	    cout << "Error: file could not be opened" << endl;
	    //exit(1);
	}

	logfile<<c.r_ip<<"  ["<<c.r_time<<"]  ["<<c.r_servetime<<"]  "<<c.r_firstline<<" "<<c.status_code<<" "<<c.r_filesize<<endl;
	logfile.close();


}

void SendData::displaylog(clientInfo c)
{

	time_t tim=time(NULL);
	tm *now=gmtime(&tim);
	char currtime[50];
	if (strftime(currtime, 50,"%x:%X", now) == 0)
		perror("Date Panga");
	string servetime(currtime);
	c.r_servetime = servetime;
	cout<<c.r_ip<<"  ["<<c.r_time<<"]  ["<<c.r_servetime<<"]  "<<c.r_firstline<<" "<<c.status_code<<" "<<c.r_filesize<<endl;

}


// Sort the list based on alphabets
bool sortDirectory(const string &left, const string &right )
{
   for( string::const_iterator lit = left.begin(), rit = right.begin(); lit != left.end() && rit != right.end(); ++lit, ++rit )
      if( tolower( *lit ) < tolower( *rit ) )
         return true;
      else if( tolower( *lit ) > tolower( *rit ) )
         return false;
   if( left.size() < right.size() )
      return true;
   return false;
}

// List the directory
void SendData::listingDir(clientInfo c)
{
	struct dirent *de=NULL;
	DIR *d=NULL;
	int last = c.r_filename.find_last_of("/");
	string dir = c.r_filename.substr(0,last);
	vector<string> dirlist;
	char * dirname = new char[dir.size() + 1];
	std::copy(dir.begin(), dir.end(), dirname);
	dirname[dir.size()] = '\0';
	d=opendir(dirname);
	if(d == NULL)
	{
		//	perror("Couldn't open directory");
		write(c.r_acceptid,"Error 404: Directory Not Found",30);
		c.status_code = 404;
	}
	else
	{
		while(de = readdir(d))
		{
			string s(de->d_name);
			dirlist.push_back(s);
		}
		vector<string>::iterator it;
		std::sort(dirlist.begin(),dirlist.end(), sortDirectory);
		write(c.r_acceptid,"Files Listing:",14);
		for ( it=dirlist.begin() ; it < dirlist.end(); it++ )
		{
			write(c.r_acceptid, (*it).c_str(), strlen((*it).c_str()));
			write(c.r_acceptid,"\n",1);
			
		}
		closedir(d);
		delete [] dirname;
		dirname = NULL;
	}


}



