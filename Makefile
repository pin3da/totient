CC=g++
CFLAGS=-std=c++11 -O3 -Wall
LDFLAGS=-lzmqpp -lzmq -lpthread -lsfml-audio -luuid
SOURCE=./src
TARGET=./bin

all: dir $(TARGET)/peer $(TARGET)/tracker $(TARGET)/t_server

dir:
	mkdir -p $(TARGET)/files
	mkdir -p $(TARGET)/pieces
	mkdir -p $(TARGET)/totient

$(TARGET)/peer: $(SOURCE)/peer.cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)/peer $(SOURCE)/peer.cc

$(TARGET)/tracker: $(SOURCE)/tracker.cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)/tracker $(SOURCE)/tracker.cc

$(TARGET)/t_server: $(SOURCE)/t_server.cc
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)/t_server $(SOURCE)/t_server.cc


clean:
	rm -rf $(TARGET)/peer $(TARGET)/tracker $(TARGET)/t_server
