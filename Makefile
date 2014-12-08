CC=g++
CFLAGS=-std=c++11 -O2 -Wall
LDFLAGS=-lzmqpp -lzmq -lpthread -lsfml-audio -luuid
SOURCE=./src

all: peer tracker

peer: $(SOURCE)/peer.cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o peer $(SOURCE)/peer.cc

tracker: $(SOURCE)/tracker.cc
	$(CC) $(CFLAGS) $(LDFALGS) -o tracker $(SOURCE)/tracker.cc


clean:
	rm -f peer tracker
