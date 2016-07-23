#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

const int REC_BUFFER = 512;
const int MSG_LIMIT = 500;
const int MIN_PORT = 0;
const int PRIVILEGED = 1024;
const int MAX_PORT = 65536;

int main(int argc, char const *argv[])
{
    if (argc < 3){
        printf("%s\n%s\n",
               "Hostname and port must be specified in command args,  example",
               "./chatclient flip2.engr.oregonstate.edu 9999"
        );
        return 1;
    }
    const char* remoteHostName = argv[1];
    const int remotePort = atoi(argv[2]);

    // assert port range, also errror when non-numeric argument supplied as atoi returns 0 and 0 is privileged
    if (remotePort < PRIVILEGED || remotePort > MAX_PORT){
        printf("invalid port number supplied (nummeric between %d-%d required)\n", PRIVILEGED, MAX_PORT);
        return 1;
    }

    printf("Client chat program launched preparing to connect to: Host:%s, port:%d\n",remoteHostName, remotePort);
    ///// INVARIANT
    ///// client launched with host string and valid port number

    //todo modulurize this, return connected socket

    // get socket file descriptor

//    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    return 0;
}

//int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);