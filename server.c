#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "4950"
#define BACKLOG 10

int main() {
    // set up a server
    int rv;
    int sockfd;
    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        printf("Error: getaddrinfo\n");
        exit(1);
    }

    // Loop through all the results to find a working domain or something, i don't really know how to word this
    // or what even happens here
    for(p = servinfo; p != NULL; p = p->ai_next) {
        // get socket descriptor: setting up the interface for network

        // socket(domain, type, protocol)
        // domain = PF_INET
        if( (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("Error: socket\n");
            continue;
        }

        // bind: necessary for servers
        if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Error: bind\n");
            continue;
        }

        break;
    }

    // test if servinfo == p is NULL (something failed, but what? failed to getaddrinfo,
    // does that mean that the hostname/IP is not available/invalid?)
    if(p == NULL) {
        printf("Error: getaddrinfo\n");
        exit(1);
    }

    // listen
    printf("listening\n");
    if(listen(sockfd, BACKLOG) == -1) {
        printf("Error: listen\n");
        exit(1);
    }

    // Exposing the IP that the client can use to send messages to this server
    char ipstr[INET6_ADDRSTRLEN];
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;
    void *addr;
    // inet_ntop(int, void *, char *, socklen_t)
    if(p->ai_family == AF_INET) {
        ipv4 = (struct sockaddr_in *)p->ai_addr;
        addr = &(ipv4->sin_addr);
    } else { // AF_INET6
        ipv6 = (struct sockaddr_in6 *)p->ai_addr;
        addr = &(ipv6->sin6_addr);
    }

    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);

    printf("Send requests to the server using this IP: %s\n", ipstr);

    while(1) {
        // accept
        // accept returns a new socket descriptor, this socket interface will be used to deal with one particular client
        // whereas the previous 'sockfd' will be used for listening for additional clients
        struct sockaddr_storage their_addr;
        socklen_t addr_size;
        printf("Waiting to accept\n");
        int newfd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);

        // process client request and send response to client
        // in this program, I will only send() "Hello World" to whoever makes a request
        printf("accepted client request, sending message to client\n");
        char *msg = "Hello world!\n";
        send(newfd, msg, strlen(msg), 0);
        printf("Message sent!\n");
    }


    printf("test\n");
}