
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DNS_PORT 53
#define NO_ATTEMPS 3
#define BUFFER_LEN 4096

int main(int argc, char **argv) {
    /*
     * argv[1]: Nome do domínio buscado
     * argv[2]: IP do servidor DNS
     * */


    // Socket struct
    struct sockaddr_in server;

    // File descriptor for socket
    int sockfd;

    int len = sizeof(server);

    // Receive buffer
    char buffer_in[BUFFER_LEN];
    // Send buffer
    char buffer_out[BUFFER_LEN];

    // Instantiate the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error on client socket creation:");
        return EXIT_FAILURE;
    }
    printf("Socket file descriptor ID: %d\n", sockfd);

    // Connection properties
    server.sin_family = AF_INET;
    server.sin_port = htons(DNS_PORT);
    server.sin_addr.s_addr = inet_addr(argv[2]);
    memset(server.sin_zero, 0x0, 8);

    // Connection attempt
    if (connect(sockfd, (struct sockaddr*) &server, len) == -1) {
        perror("Can't connect to server");
        return EXIT_FAILURE;
    }

    // Zeroing the buffers
    memset(buffer_in, 0x0, BUFFER_LEN);
    memset(buffer_out, 0x0, BUFFER_LEN);

    fprintf(stdout, "Say something to the server: ");
    fgets(buffer_out, BUFFER_LEN, stdin);

    // Sends the read message to the server through the socket
    send(sockfd, buffer_out, strlen(buffer_out), 0);

    // Receives an answer from the server
    recv(sockfd, buffer_in, BUFFER_LEN, 0);
    printf("Server answer: %s\n", buffer_in);

    return 0;
}
