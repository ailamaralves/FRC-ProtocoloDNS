

/*
+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 | PREFERENCE                                   |
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 / EXCHANGE                                     /
 /                                              /
 +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

Onde:
PREFERENCE Um inteiro de 16 bits que especifica a preferência dada a
 este RR entre outros no mesmo proprietário. Valores mais baixos
 são preferidos.
EXCHANGE A <domain-name> que especifica um host disposto a agir como
 uma troca de correio para o nome do proprietário.
Os registros MX causam processamento de seção adicional do tipo A para o host
especificado por EXCHANGE. O uso de MX RRs é explicado em detalhes em
*/

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
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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
    if( (send(sockfd, buffer_out, strlen(buffer_out), 0)) < 0){
        perror("send");
        close(sockfd);
        return EXIT_FAILURE;
    }

    // Receives an answer from the server
    int trys = 3, answerlen = 1;
    while (trys-- > 0){
        answerlen = recv(sockfd, buffer_in, BUFFER_LEN, 0);
        printf("Server answer: %s\n", buffer_in);
    }


    close(sockfd);

    return 0;
}
