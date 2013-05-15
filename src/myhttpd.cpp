//============================================================================
// Name        : myhttpd.cpp
// Author      : Himank Chaudhary
// Version     :
// Copyright   : Your copyright notice
// Description : Main File
//============================================================================
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../src/myhttpd.h"
#include "../src/myserver.h"
#include "../src/parse.h"
#include <pthread.h>
#include <signal.h>
#include<string>
#include <cstring>
#include <unistd.h>


string port = "8080";
bool r_daemon = false;
bool logging = false;
string scheduling ="FCFS";
int threadnum = 10;
bool summary = false;
int r_time = 30;
string l_file ="log.txt";
string rootdir ="/home/himank/workspace/TeamG14/files";

using namespace std;


//bool r_daemon = false;
int sockId =0;
Parse *P = new Parse();
pthread_mutex_t rqueue_lock;
pthread_cond_t rqueue_cond;
pthread_mutex_t print_lock;
pthread_cond_t print_cond;
pthread_t thread_scheduler;
pthread_t threads[30];
RunServer *run = new RunServer();
sig_atomic_t flag =0;

void signal_callback_handler(int signum)
{
   //close(sockId);
   //delete P;
   //P= null;
   //exit(signum);
	flag++;
	close(sockId);
	delete P;
	P = NULL;
	delete run;
	run = NULL;
	pthread_exit(NULL);
	exit(signum);

}
void printh()
{
cout<<endl<<"**************************************************************************"<<endl;
cout<<"Usage Summary: myhttpd -d -h -l filename -p portno -r rootdirectory -t threadwaittime -n threadnumber -s scheduling"<<endl<<endl;
cout<<"Give -d parameter in args to on debugging mode"<<endl;
cout<<"Give -h parameter to display the summary"<<endl;
cout<<"Give -l and then filename of logging file for example: logging.txt"<<endl;
cout<<"Give -p and then portno to change default port number for example: -p 8080"<<endl;
cout<<"Give -r and then root directory to change default value of root directory for example: -r home/workspace/Myhttpd/"<<endl;
cout<<"Give -t and then thread time to change default wait time of scheduler thread for example: -t 30"<<endl;
cout<<"Give -n and then thread numbers to change the default value of threads for example: -n 10"<<endl;
cout<<"Give -s and then scheduling name to change default scheduling for example: -s SJF"<<endl;
cout<<"Press Ctrl+c anytime to exit the server";
cout<<endl<<"**************************************************************************"<<endl;
exit(1);
}
int main( int argc, char *argv[] )
    {

        int opt = 0;

        opt = getopt( argc, argv,"dhl:p:r:t:n:s:" );
        while( opt != -1 ) {
            switch( opt ) {
                case 'd':
                     r_daemon = true;
                    break;
                    
                case 'h':
                      summary = true;
                    break;
                    
                case 'l':
                     l_file.assign(optarg);
                	 logging = true;
                    break;
                    
                case 'p':
                     port.assign(optarg);
                    break;
                    
                case 'r':
                     rootdir.assign(optarg);
                    break;
                    
                case 's':
                     scheduling.assign(optarg);
                    break;
                    
                case 't':
                    r_time =atoi(optarg);
                    break;
                    
                case 'n':
                     threadnum=atoi(optarg);
                    break;

                    
                default:
                    break;
            }
            
            opt = getopt( argc, argv, "dhl:p:r:t:n:s:" );
            
        }


    if(r_daemon)
	{
		run->accept_connection();
	}
    if(summary)
    {
    	printh();
    }

    else
	{

		if (pthread_mutex_init(&rqueue_lock, NULL) != 0)
		{
				printf("\n mutex init failed\n");
				return 1;
		}
		pthread_mutex_init(&print_lock, NULL);
		pthread_cond_init (&rqueue_cond, NULL);
		pthread_cond_init (&print_cond, NULL);
		pthread_create(&thread_scheduler,NULL,&Parse::popRequest_helper,P);
		for(int i =0;i<threadnum;i++)
				pthread_create(&threads[i],NULL,&Parse::serveRequest_helper,P);
		run->accept_connection();
		pthread_mutex_destroy(&rqueue_lock);
		pthread_cond_destroy(&rqueue_cond);
		pthread_mutex_destroy(&print_lock);
		pthread_cond_destroy(&print_cond);
		pthread_exit(NULL);
	}

	signal(SIGINT, signal_callback_handler);
	close(sockId);
	delete P;
	P = NULL;
	delete run;
	run = NULL;
	return 0;
}
