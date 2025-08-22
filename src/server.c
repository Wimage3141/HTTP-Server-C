#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT "4950"
#define BACKLOG 10
#define MAXBUFSIZE 200

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

        // Allowing fast restart after closing a server so that prevent a
        // 'address already in use' error
        int yes = 1;
        if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            printf("Error: set socket option\n");
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

    while(1) {
        // accept
        // accept returns a new socket descriptor, this socket interface will be used to deal with one particular client
        // whereas the previous 'sockfd' will be used for listening for additional clients
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        printf("Waiting to accept\n");
        int newfd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);

        // process client request and send response to client
        // in this program, I will only send() "Hello World" to whoever makes a request
        char buf[MAXBUFSIZE];
        int n = recv(newfd, buf, sizeof buf, 0);
        if(n > 0) {
            if(n > MAXBUFSIZE) {
                printf("Length exceeded.\n");
                // send a bad signal.
                continue;
            }
            buf[n] = '\0';
        }
        else if(n == 0) {
            printf("Server is closed\n");
        }
        else {
            printf("Error: recv");
        }

        printf("First HTTP formatted request incoming? Exciting!\n");
        printf("The actual HTTP formatter request: \n%s\n", buf);
        memset(buf, 0, sizeof buf);


        char msg[] = (
            "Would you like Fried chicken or Poutine?\n"
            "Please enter 1 for fried chicken\n"
            "Enter 2 for poutine\n"
        );
        if(send(newfd, msg, strlen(msg) + 1, 0) == -1) {
            printf("Error: sending the initial prompt to the client\n");
            printf("Don't exit to keep the server running\n");
        }
        printf("Message sent!\n\n");

        // receive the decision
        n = recv(newfd, buf, sizeof buf, 0);

        printf("test: what's n? %d\n", n);
        
        if(n > 0) {
            if(n > 1) {
                printf("Response length exceeded, did not select '1' or '2'\n");
                // send an indicator that'll signal that something is wrong with the user input
                // instead of making a custom one just for this case, I'll move on to http since that is exactly 
                // what http is for! (among other things)
                continue;
            }

            buf[n] = '\0';
        }
        else if(n == 0) {
            printf("Server closed\n");
        }
        else {
            printf("Error: recv");
        }

        printf("Ahh, great choice! (%s)\n", buf);

        // Sending the message according to the client response
        // memset(msg, 0, sizeof msg);
        char client_choice[20];
        memset(client_choice, 0, sizeof client_choice);

        if(strcmp(buf, "1") == 0) {
            strcpy(client_choice, "Fried chicken");
        }
        else if(strcmp(buf, "2") == 0) {
            strcpy(client_choice, "Poutine");
        }
        else {
            printf("Please select a valid response (Either 1 or 2).\n");
        }


        printf("You have chosen: %s. \nEnjoy!\n", client_choice);
        
        if(send(newfd, client_choice, strlen(client_choice) + 1, 0) == -1) {
            printf("Error: sending client choice response\n");
            exit(1);
        }

        close(newfd);
    }
    close(sockfd);
}