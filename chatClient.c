#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>


const int REC_BUFFER = 512;
const int MSG_LIMIT = 500;
const int MIN_PORT = 0;
const int PRIVILEGED = 1024;
const int MAX_PORT = 65536;
const int GET_ADDR_NO_ERROR = 0;
const int CONNECT_ERROR = -1;

int main(int argc, char const *argv[])
{
    if (argc < 3){
        fprintf(stderr, "%s\n%s\n",
               "Hostname and port must be specified in command args,  example",
               "./chatclient flip2.engr.oregonstate.edu 9999"
        );
        return 1;
    }
    const char* remoteHostName = argv[1];
    const int remotePortNum = atoi(argv[2]);
    const char* remotePortSt = argv[2];

    // assert port range, also errror when non-numeric argument supplied as atoi returns 0 and 0 is privileged
    if (remotePortNum < PRIVILEGED || remotePortNum > MAX_PORT){
        fprintf(stderr, "invalid port number supplied (nummeric between %d-%d required)\n", PRIVILEGED, MAX_PORT);
        return 1;
    }

    printf("Client chat program launched preparing to connect to: Host:%s, port:%d\n",remoteHostName, remotePortNum);
    ///// INVARIANT
    ///// client launched with host string and valid port number

    //todo modulurize this, return connected socket


    // set up hints struct and other socket initialization referenced from Beej's guide and linux manual examples
    // http://beej.us/guide/bgnet/output/html/multipage/clientserver.html
    // http://man7.org/linux/man-pages/man3/getaddrinfo.3.html

    // set up structs for requesting ip address from hostname
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int getadderResult = getaddrinfo(remoteHostName, remotePortSt, &hints, &servinfo);
    if (getadderResult != GET_ADDR_NO_ERROR){
        printf(stderr, "getaddrinfo call failed with error: %s\n", gai_strerror(getadderResult));
        return 1;
    }

    // create socket file descriptor
    //int s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    for (struct addrinfo *rp = servinfo; rp != NULL; rp = rp->ai_next) {
        // create file descriptor
        fprintf(stderr, "attempting to connect to server on using addrinfo struct at:%p\n", rp);
        int s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (s == -1){
            fprintf(stderr, "%s\n","failed to get socket file descriptor");
            continue;
        }

        // connect socket to server
        if (connect(s, rp->ai_addr, rp->ai_addrlen) != CONNECT_ERROR){
            fprintf(stderr, "socket connected to server");
            break;
        }
        close(s);
    }




//    // get socket file descriptor
//    int s = socket(AF_INET, SOCK_STREAM, 0);
//    if (s == -1){
//        fprintf(stderr, "%s\n","(CRITICAL ERROR) Could not create socket");
//        return 1;
//    }
//
//    printf("%s\n","Socket created");
//
//    struct sockaddr_in server;
//    server.sin_family = AF_INET;
//    server.sin_addr.s_addr = htons()


    return 0;
}

//int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);