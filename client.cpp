/*
    client.cpp
    1. Build a TCP connection to a server, a machine alias and port number is required.
    2. After the connection is established, use client_process to demonstrate the transmission of data.
*/
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Message.h"

const int BYTE_SIZE = 64; //Maximum bytes needed for all messages
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
    int n = write(sockfd, buffer, BYTE_SIZE);
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
void client_process(int sockfd);

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

    /* Mock sending an order message then recive a response object */
    client_process(sockfd);

    close(sockfd);
    return 0;
}

void client_process(int sockfd)
{
    /* Sending buy and sell thresholds. */
    uint8_t buffer[BYTE_SIZE];
    Threshold threshold;
    threshold.get_threshold();
    threshold.serialize(buffer);
    send_buffer(buffer, sockfd);

    /* Sending a sell order. */
    int order_id = 0;
    int sequence_num = 0;
    Header send_header(AF_INET, 35, sequence_num++); //NewOrder is size 35;
    NewOrder send_order(FLOW, order_id++, 15, 1500, 's');
    send_header.serialize(buffer);
    send_order.serialize(buffer);
    send_buffer(buffer, sockfd);

    /* Reciving the reponse regarding that order. */
    recive_buffer(buffer, sockfd);
    Header recived_header;
    OrderResponse order_response;
    recived_header.deserialize(buffer);
    order_response.deserialize(buffer);
    std::cout << "----Recived header info----\n";
    std::cout << recived_header;
    std::cout << "----Recive order_response info----\n";
    std::cout << order_response;
}