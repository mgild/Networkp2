CC = g++
CFLAGS = -g -Wall -std=c++11

all: miProxy nameserver

miProxy: Server.cpp main.cpp Socket.cpp HTTPHandler.cpp stringutils.cpp logger.cpp
	$(CC) $(CFLAGS) Server.cpp main.cpp HTTPHandler.cpp Socket.cpp \
	   stringutils.cpp logger.cpp	-o miProxy

nameserver: Server.cpp nameserver.cpp Socket.cpp HTTPHandler.cpp stringutils.cpp logger.cpp
	$(CC) $(CFLAGS) Server.cpp nameserver.cpp Socket.cpp HTTPHandler.cpp stringutils.cpp logger.cpp \
		-o nameserver

clean:
	-rm -f *.o *~ *core* server miProxy
