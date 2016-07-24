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


const int REC_BUFFER_SIZE = 512;
const int MSG_LIMIT = 500;
const int HANDLE_LIMIT = 10;
const int MIN_PORT = 0;
const int PRIVILEGED = 1024;
const int MAX_PORT = 65536;
const int GET_ADDR_NO_ERROR = 0;
const int CONNECT_ERROR = -1;
const char* QUIT = "\\quit";
#define MIN(a,b) (((a)<(b))?(a):(b))


int getConnectedSocket(const char* remoteHostName, const char* remotePortSt){
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
        return 0;
    }

    // create socket file descriptor
    //int s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    int s = 0;
    for (struct addrinfo *rp = servinfo; rp != NULL; rp = rp->ai_next) {
        // create file descriptor
        dprintf(3, "attempting to connect to server on using addrinfo struct at:%p\n", rp);
        s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (s == -1){
            dprintf(3, "%s\n","failed to get socket file descriptor");
            continue;
        }

        // connect socket to server
        if (connect(s, rp->ai_addr, rp->ai_addrlen) != CONNECT_ERROR){
            dprintf(3, "%s\n", "socket connected to server");
            break;
        }
        close(s);
    }
    // free up memory used by addr-struct-linked-list
    freeaddrinfo(servinfo);

    return s;
}

void chat(int sock, const char* handle){
    dprintf(3, "%s\n", "beginning chat program");
    //todo remvoe temp
    char** msgarray = malloc(sizeof(char*) * 4);
    msgarray[0] = "hello";
    msgarray[1] = "i am client";
    msgarray[2] = "goodbye";
    msgarray[3] = "\\quit";
    char* msg = *msgarray;

    char* readBuffer = malloc(REC_BUFFER_SIZE + 1);

    while (strncmp(msg, QUIT, MIN(strlen(msg), strlen(QUIT)))) {
        size_t len = strlen(msg) + 1;
        /* +1 for terminating null byte */

        if (len + 1 > MSG_LIMIT) {
            //todo truncate message and inform user
            // or transmit in parts

        }

        //todo move out of if
        if (write(sock, msg, len) != len) {
            //todo better error
            fprintf(stderr, "partial/failed write\n");
            //exit(EXIT_FAILURE);
        }

        size_t nread = (size_t) read(sock, readBuffer, REC_BUFFER_SIZE);
        if (nread == -1) {
            perror("read");
            //exit(EXIT_FAILURE);
        }

        printf("Received %ld bytes: %s\n", (long) nread, readBuffer);


        //todo remove temp msg
        msgarray +=1;
        msg = *msgarray;
    }
}

char* getHandle(){
    fseek(stdin,0,SEEK_END);
    char* handle = malloc(sizeof(char*) * (HANDLE_LIMIT + 1));
    fgets(handle, HANDLE_LIMIT, stdin);
    for (char* c= handle; c-handle < HANDLE_LIMIT + 1; ++c){
        putc(*c,stdout); putc('\n', stdout);
    }
    //consume extra chars from input buffer
    while((getchar())!='\n');
    return "CLIENT>";
}

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

    printf("Client chat program launched preparing to connect to:\nHost:%s, port:%d\n",remoteHostName, remotePortNum);
    ///// INVARIANT
    ///// client launched with host string and valid port number

    int sock = getConnectedSocket(remoteHostName, remotePortSt);
    if (!sock){
        fprintf(stderr, "%s\n", "(CRITICAL ERROR) failed to connect to server");
        return 1;
    }

    ///// INVARIANT
    ///// client script has connected to server using socket sock

    //chat(sock, getHandle());
    getHandle();
    getHandle();

    // client has exited with \quit command, close socket
    close(sock);

    dprintf(3,"%s\n", "Disconnected from server");

    return 0;
}

