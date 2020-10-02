#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORTA 53
#define ERRO -1
#define TAMMAX 250     // tamanho maximo da string

int main () {

 struct sockaddr_in network,local;
 int sock,
    newSock,
    resp,
    strucsize,pros;

 char msgbuffer [TAMMAX];

    if(fork() == 0){
        sock = socket (AF_INET, SOCK_DGRAM, 0);
        if (sock == ERRO) {
        perror ("Socket");
        exit (0);
        }
        bzero ((char *)&local, sizeof (local));
        local.sin_family = AF_INET;
        local.sin_port = htons (PORTA);
        local.sin_addr.s_addr = INADDR_ANY;
        strucsize = sizeof (local);
        resp = bind (sock, (struct sockaddr *)&local, strucsize);

        if (resp == ERRO) {
        perror ("Bind");
        exit (0);
        }

        // numero maximo de conexões agente definiu aqui
        listen (sock, 5);
        for(;;) {			
            if((newSock = accept (sock, (struct sockaddr *)&network, &strucsize))==1) {
                perror("accept");
                exit(1);
            }

            if (newSock == ERRO) {
            perror ("Accept");
            exit (0);
            } 
            if(!fork()) {
            printf ("Recebendo conexao de: %s\n", inet_ntoa (network.sin_addr));
            //permite o cliente da uma entrada e a mostra, se a entrada for exit bula o laço "for" infinito
                for (;;) {
                recv (newSock, msgbuffer, TAMMAX, 0);
                fprintf (stdout, "\nMensagem Recebida: %s\n", msgbuffer);
                if (!strcmp (msgbuffer, "exit")) break;
                }
            }
        }
    }

 return 0;

}