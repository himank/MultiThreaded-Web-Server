#ifndef MYHTTPD_H_
#define MYHTTPD_H_

#include "../src/parse.h"
#include <pthread.h>
#include "../src/myserver.h"
extern pthread_mutex_t rqueue_lock;
extern pthread_cond_t rqueue_cond;
extern pthread_mutex_t print_lock;
extern pthread_cond_t print_cond;
extern Parse *P;
class RunServer;
extern int sockId;
extern bool r_daemon;
extern RunServer *run;

extern string port;
extern bool logging;
extern string scheduling;
extern int threadnum;
extern bool summary;
extern int r_time;
extern string l_file;
extern string rootdir;



#endif // ALLHEADERS_H_


