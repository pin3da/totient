CC=g++
CFLAGS=-std=c++11 -O2 -Wall
LDFLAGS=-lzmqpp -lzmq -lpthread -lsfml-audio -luuid
SOURCE=./src

all: node tracker

node: $(SOURCE)/node.cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o node $(SOURCE)/node.cc

tracker: $(SOURCE)/tracker.cc
	$(CC) $(CFLAGS) $(LDFALGS) -o tracker $(SOURCE)/tracker.cc


clean:
	rm -f node tracker
