/*
    server.cpp
    1. Creating a server to enable TCP connection, a port number needs to be passed in as an argument.
    2. After a client has connected, run a pre-determined server_process() to demonstrate the transmission 
        of data.
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

const int BYTE_SIZE = 64; //Maximum bytes needed for all messages.
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void send_buffer(uint8_t *buffer, int sockfd)
{
    // Send buffer
    int n = write(sockfd, buffer, BYTE_SIZE);
    if (n < 0)
        error("ERROR writing to socket");
}

void recive_buffer(uint8_t *buffer, int sockfd)
{
    // Recive response.
    bzero(buffer, BYTE_SIZE);
    int n = read(sockfd, buffer, BYTE_SIZE);
    if (n < 0)
        error("ERROR reading from socket");
}

void server_process(int sock);

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

    /* Mock reciveing an order message then send back response object */
    server_process(newsockfd);

    close(newsockfd);
    return 0; /* we never get here */
}

/******** server_process() *********************
It recives threshold infomation to build create a portfolio objects, 
once a connnection has been established. And run a simple data transmission.
 *****************************************/
void server_process(int sock)
{
    // Recive threshold info.
    uint8_t buffer[BYTE_SIZE];
    recive_buffer(buffer, sock);
    Threshold threshold;
    threshold.deserialize(buffer);
    std::cout << "----Recived threshold info----\n";
    std::cout << threshold;

    // Create portfolio.
    int sequence_num;
    Portfolio portfolio(threshold.buy_threshold, threshold.sell_threshold);

    // Recive Order Message.
    recive_buffer(buffer, sock);
    Header recived_header;
    NewOrder recived_order;
    recived_header.deserialize(buffer);
    sequence_num = recived_header.sequenceNumber;
    recived_order.deserialize(buffer);
    std::cout << "----Recived header info----\n";
    std::cout << recived_header;
    std::cout << "----Recived order info----\n";
    std::cout << recived_order;

    Header send_header(AF_INET, 12, sequence_num++); //OrderResponse is size 12;
    OrderResponse order_response = portfolio.place_order(recived_order);
    send_header.serialize(buffer);
    order_response.serialize(buffer);
    send_buffer(buffer, sock);
}