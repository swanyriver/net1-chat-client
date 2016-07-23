CFLAGS = -std=c99 -Wall

chatclient: chatClient.c
	gcc chatClient.c -o chatclient $(CFLAGS)
