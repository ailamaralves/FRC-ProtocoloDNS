#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORTA 53
#define ERRO -1
#define TAMMAX 250     //tamanho maximo da string

int main (int argc, char * * argv) {

    struct sockaddr_in network;

    int sock,
        newSock,
        resp,
        strucsize;

    char msg [TAMMAX];

    if (argc < 2) {
        printf ("Use %s <host>\n\n", argv [0]);
        exit (0);
    }

    sock = socket (AF_INET, SOCK_DGRAM, 0);

    if (sock == ERRO) {
        perror ("Socket");
        exit (0);
    }

    bzero ((char *)&network, sizeof (network));
    network.sin_family = AF_INET;
    network.sin_port = htons (PORTA);
    network.sin_addr.s_addr = inet_addr (argv [1]); 

    strucsize = sizeof (network);

    resp = connect (sock, (struct sockaddr *)&network, strucsize);

    if (resp == ERRO) {
        perror ("Connect");
        exit (0);
    }

    fprintf (stdout, "Conectado em %s\n", argv [1]);

    for (;;) {
        printf ("\nMensagem: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strlen(msg)-1] = '\0';
        send (sock, msg, sizeof (msg), 0);
        if (!strcmp (msg, "exit")) {
            exit (0);
        }
    }

    return 0;
}
