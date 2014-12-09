CC=g++
CFLAGS=-std=c++11 -O2 -Wall
LDFLAGS=-lzmqpp -lzmq -lpthread -lsfml-audio -luuid
SOURCE=./src
TARGET=./bin

all: dir $(TARGET)/peer $(TARGET)/tracker

dir:
	mkdir -p $(TARGET)/files
	mkdir -p $(TARGET)/pieces

$(TARGET)/peer: $(SOURCE)/peer.cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)/peer $(SOURCE)/peer.cc

$(TARGET)/tracker: $(SOURCE)/tracker.cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)/tracker $(SOURCE)/tracker.cc


clean:
	rm -rf $(TARGET)/peer $(TARGET)/tracker
