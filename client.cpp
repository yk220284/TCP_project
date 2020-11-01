// #include <stdio.h>
// #include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Message.h"

enum STOCKS
{
    FLOW,
    OTHER,
};

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
    int n = read(sockfd, buffer, 255);
    if (n < 0)
        error("ERROR reading from socket");
}

void process(int sockfd)
{
    int order_id = 0;
    int sequence_num = 0;
    uint8_t buffer[64];
    Header hearder1(AF_INET, 35, sequence_num++); //NewOrder is size 35;
    NewOrder order1(FLOW, order_id++, 15, 1500, 's');
    hearder1.serialize(buffer);
    order1.serialize(buffer);
    send_buffer(buffer, sockfd);
}

int main(int argc, char *argv[])
{
    /* Establish TCP connection. */
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* Sending buy and sell thresholds. */
    uint8_t buffer[16];
    Threshold threshold;
    threshold.get_threshold();
    threshold.serialize(buffer);
    send_buffer(buffer, sockfd);
    // process(sockfd);
    // while (1)
    // {

    close(sockfd);
    return 0;
}
