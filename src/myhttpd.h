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


