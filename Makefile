CC = g++
CFLAGS = -g -Wall -std=c++11

all: miProxy

miProxy: Server.cpp main.cpp Socket.cpp HTTPHandler.cpp stringutils.cpp
	$(CC) $(CFLAGS) Server.cpp main.cpp HTTPHandler.cpp Socket.cpp \
	   stringutils.cpp	-o miProxy

client: client.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm -f *.o *~ *core* server miProxy
