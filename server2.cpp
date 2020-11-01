/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Message.h"

void dostuff(int); /* function prototype */
void read_header(int);
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2)
    {
        error("ERROR no port provided");
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    close(sockfd);
    while (true)
    {
        // dostuff(newsockfd);
        read_header(newsockfd);
    } /* end of while */
    close(newsockfd);
    return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff(int sock)
{
    int n;
    char buffer[256];

    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
    printf("Here is the message: %s\n", buffer);
    n = write(sock, "I got your message", 18);
    if (n < 0)
        error("ERROR writing to socket");
}

void read_header(int sock)
{
    int n;
    uint8_t buffer_size = 16;
    uint8_t buffer[buffer_size];
    bzero(buffer, buffer_size);

    n = read(sock, buffer, buffer_size);
    if (n < 0)
        error("ERROR reading from socket");
    Header h1;
    h1.deserialize(buffer);
    std::cout << h1;
    n = write(sock, "I got your message", 18);
    if (n < 0)
        error("ERROR writing to socket");
}