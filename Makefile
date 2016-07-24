CFLAGS =  -Wall -std=gnu99

chatclient: chatClient.c
	gcc chatClient.c -o chatclient $(CFLAGS)
