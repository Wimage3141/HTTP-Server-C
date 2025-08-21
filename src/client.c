#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LOCAL_IP "127.0.0.1"
#define PORT "4950"
#define MAXBUFSIZE 200

void *get_in_addr(struct addrinfo *addrinfo) {
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;
    void *in_addr;

    if(addrinfo->ai_family == AF_INET) {
        ipv4 = (struct sockaddr_in *)addrinfo->ai_addr;
        in_addr = &(ipv4->sin_addr);
    }
    else {
        ipv6 = (struct sockaddr_in6 *)addrinfo->ai_addr;
        in_addr = &(ipv6->sin6_addr);
    }

    return in_addr;
}

void validate_recv_response(ssize_t n, char **buf) {
    if(n == 0) {
        printf("Server closed\n");
    }
    else if(n > 0) {
        buf[n] = '\0';
    }
    else {
        printf("Error: while recv() ing\n");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Please provide the server IP as an argument!\n");
        return -1;
    }

    int rv;
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int sockfd;

    if( (rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        printf("Error: getaddrinfo\n");
        return 1;
    }

for(p = servinfo; p != NULL; p = p->ai_next) {
        // socket(domain, type, protocol)
        // domain = PF_INET
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("Error: socket\n");
            continue;
        }

        // conenct to server
        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            printf("Error: connect\n");
            continue;
        }

        break;
    }

    // check if servinfo is valid
    if(p == NULL) {
        printf("Error: getaddrinfo could not populate servinfo");
        exit(1);
    }
    // const char * inet_ntop(int, const void *, char *, socklen_t)
    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop(
        p->ai_family,
        get_in_addr(p),
        ipstr,
        sizeof ipstr
    );

    // In my toy server, the server talks first, but in HTTP
    printf("Sending request to server: %s\n", ipstr);

    char buf[MAXBUFSIZE];
    ssize_t recv_msg_last_idx = recv(sockfd, buf, sizeof buf, 0);
    validate_recv_response(recv_msg_last_idx, &buf);

    printf("Server response: %s\n", buf);

    char response[10];
    scanf("%s", response);

    if(send(sockfd, response, strlen(response), 0) == -1) {
        printf("Error: sending choice to server\n");
        exit(1);
    }
    printf("Decision sent!\n");

    memset(buf, 0, sizeof buf);
    recv_msg_last_idx = recv(sockfd, buf, sizeof buf, 0);

    validate_recv_response(recv_msg_last_idx, &buf);

    printf("Here is your order: %s\n", buf);

    close(sockfd);
}