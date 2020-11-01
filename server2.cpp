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
#include "Portfolio.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void send_buffer(uint8_t *buffer, int sockfd)
{
    // Send buffer
    int n = write(sockfd, buffer, 64);
    if (n < 0)
        error("ERROR writing to socket");
}

void recive_buffer(uint8_t *buffer, int sockfd)
{
    // Recive response.
    bzero(buffer, 64);
    int n = read(sockfd, buffer, 64);
    if (n < 0)
        error("ERROR reading from socket");
}

void dostuff(int); /* function prototype */
void handle_message(int sock);
int main(int argc, char *argv[])
{
    /* Establish TCP connection. */
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

    /* Recive buy and sell thresholds. */
    // while (true)
    // {
    // dostuff(newsockfd);
    handle_message(newsockfd);
    // } /* end of while */
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

void handle_message(int sock)
{
    // int n;
    uint8_t buffer_size = 64;
    uint8_t buffer[buffer_size];
    bzero(buffer, buffer_size);
    recive_buffer(buffer, sock);
    Threshold threshold;
    threshold.deserialize(buffer);
    Portfolio portfolio(threshold.buy_threshold, threshold.sell_threshold);

    // while (true)
    // {
    // }
    std::cout << threshold;
    // recive_buffer(buffer, sock);
    // Header header1;
    // NewOrder order1;
    // header1.deserialize(buffer);
    // order1.deserialize(buffer);

    // std::cout << header1;
    // std::cout << order1;

    // send_buffer("I got your message", sock);
    // n = write(sock, "I got your message", 18);
    // if (n < 0)
    // error("ERROR writing to socket");
}
