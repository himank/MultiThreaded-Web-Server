objects = myhttpd.o myserver.o senddata.o parse.o

myhttpd: $(objects)
	g++ -o myhttpd $(objects) -lpthread

myhttpd.o: myhttpd.h myserver.h parse.h

myserver.o: myserver.h myhttpd.h parse.h

senddata.o: senddata.h parse.h myhttpd.h

parse.o: parse.h myhttpd.h senddata.h


.PHONY: clean
clean:
	-rm edit $(objects)
