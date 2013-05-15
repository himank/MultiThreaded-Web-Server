Multithreaded Web Server: It is a web server implemented in  C++ on a UNIX--?based platform so that it can server multiple incoming client requests.

Overview:
The  server reads a request (GET or HEAD) from the client and serves the content from the current directory. By default the server is handling multiple requests
i.e. queuing thread reads requests from the given port and places the requests in ready
queue. Scheduling thread reads the request from the ready queue and based on the scheduling policy issues the request to one of the available worker threads. The worker
thread then serves the response back to the client and closes the connection. 

If debug mode is set through the command line then the server will handle only one request at a time.

Moreover, in case of Head request file size should be taken zero and only file metadata information should be returned. In case of Get request whole file content should be displaced.

if the request is for a particular file and that file does not exist then the listing of directory is displaced based on alphabetic order.

Logging of request is done in a file.

Two scheduling policies are used to serve the client request:
1. First come first cout.
2. Shortest Job FIrst.

A threadpool of initial size (10) is set up at the starting + one scheduler thread is used + main thread.

By default file directory is set to : "/home/himank/workspace/TeamG14/files" which should be changed based on where the files are present.


Command line options:

To run:

myhttpd [?d] [?h] [?l file] [?p port] [?r dir] [?t time] [?n threadnum] [?ssched]

-d -> It will run in debugging mode.

-h -> It will show the summary and exit.

-l file-> It will switch on the logging functionality and use the filename which you will specify in the parameters. Format(eg: /home/timberlake/cse521/log.txt)

-p portno -> It will change the by default value of port no 8080 to the one you specified.

-r directory -> Please specify the path here as by default root directory of my system is set. In format(eg: /home/timberlake/cse521/TeamG14 )

-t time -> It will change the by default wait time of scheduler thread from 30 seconds to one you specify.

-n threadnum -> It will change the by default threadpool size of 10 to the number you specify here.

-s scheduling -> By default it is "FCFC", specify "SJF" here to change the scheduling.
