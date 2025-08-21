#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
// #include <netinet/in.h>

int main(int argc, char *argv[]) {
    struct addrinfo hints, *res, *p;

    if(argc != 2) {
        printf("provide the hostname via cli argument\n");
        exit(1);
    }

    // fills hints with byte 0 so that all int point to 0 and all pointers
    // point to the address ...00000... (on modern computers, it's just NULL)
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // If set to AI_PASSIVE, it binds to the IP of the host running the program
    // If that IP has to be something else then change the first argument of the getaddrinfo()
    // function from NULL to something the 'target' like www.google.com (as I've done below with argv[1])
    // hints.ai_flags = AI_PASSIVE;

    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if(status = getaddrinfo(argv[1], NULL, &hints, &res) != 0) {
        printf("error calling getaddrinfo\n");
        exit(1);
    } 

    for(p = res; p != NULL; p = p->ai_next) {
        // it's void so that it can point so it can point to both
        // struct in_addr and struct in6_addr without casting
        void *addr;
        char *ipver;
        struct sockaddr_in *ipv4;
        struct sockaddr_in6 *ipv6;

        if(p->ai_family == AF_INET) {
            ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        
        else { // AF_INET6
            ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("%s: %s\n", ipver, ipstr);

        // socket stuff
        int sockfd;
        // socket() returns a socket descriptor (which is basically like a file descriptor)
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol) == -1)) {
            printf("Error while setting up the socket\n");
            exit(1);
        }

        // bind stuff
        // only relevant for a server, like when trying to listen() for a network call
        // not needed for a client making a connect() call

        // ok more insight, bind is only required when we have to use a specific port (to 
        // tell other people which port to conenct to!)
        // and this is important for hosting a server!
        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Error while binding\n");
            exit(1);
        }

        // connect

        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            printf("");
        }
    }

    freeaddrinfo(res);
}