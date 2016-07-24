CFLAGS =  -Wall -std=gnu99

all: chatclient

chatclient: chatClient.c
	gcc chatClient.c -o chatclient $(CFLAGS)
