

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

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <errno.h>
// #include <stdbool.h>

// #include <sys/types.h>
// #include <sys/socket.h>

  
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
 
#define MX 15

//Function Prototypes
void ChangetoDnsNameFormat (unsigned char*, char*);
 
//DNS header structure
struct dns_header{
  unsigned short transaction_ID; 
  unsigned short flags;
  unsigned short questions; 
  unsigned short answer_RRs; 
  unsigned short authority_RRs; 
  unsigned short additional_RRs; 
};

struct query{
  char* name;
  unsigned short qtype;
  unsigned short qclass;
};


#define DNS_PORT 53
#define NO_ATTEMPS 3
#define BUFFER_LEN 4096
#define MX 15
#define IN 1

/*
* argv[1]: Nome do domínio buscado
* argv[2]: IP do servidor DNS
* */

int main(int argc, char **argv) {



  // Socket struct
  struct sockaddr_in server;

  // File descriptor for socket
  int sockfd;
  int hostlen;
  for(hostlen = 0; argv[1][hostlen] != '\0'; hostlen++);

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
  
  struct dns_header header;
  memset (&header, 0, sizeof(struct dns_header));
  
  //segundo as recomendaçoes
  //id = rand(), flag = 0100, questions = 0001 e os demais 0000
  srand(time(NULL));
  unsigned short rand_id =  

  header.transaction_ID= htons(rand_id);   
  header.flags = htons(0x0100);        
  header.questions = htons(0x0001);    
  header.answer_RRs = htons(0x0000); 
  header.authority_RRs = htons(0x0000);
  header.additional_RRs = htons(0x0000); 

  //a.com.br 1a3com2br a a. as.com
  int count;
  char* domain_name = calloc(hostlen + 2, sizeof(char));
  for(int i = 0; i < hostlen; i++){
    for(count = 0; argv[1][i] != '.' && i < hostlen; i++, count++); 
    domain_name[i - count] = count;
    strncat(domain_name, argv[1] + (i - count), count);
  }
  for(int i = 0; i < hostlen+2; i++){
    printf("%0x ", domain_name[i]);
  }
  printf("\n");

  struct query queries;
  queries.name = calloc(hostlen + 2, sizeof(char));
  strcpy(queries.name, domain_name);
  queries.qtype = htons(MX);
  queries.qclass = htons(IN);


  // Connection attempt
  if (connect(sockfd, (struct sockaddr*) &server, sizeof(server)) == -1) {
    perror("Can't connect to server");
    return EXIT_FAILURE;
  }

  // Zeroing the buffers
  memset(buffer_in, 0x0, BUFFER_LEN);
  memset(buffer_out, 0x0, BUFFER_LEN);


  fprintf(stdout, "Say something to the server: ");
  fgets(buffer_out, BUFFER_LEN, stdin);

  unsigned aswrlen = sizeof(header) + (sizeof(domain_name)) +
    sizeof(queries.qtype) + sizeof(queries.qclass);
  unsigned char* data = calloc(aswrlen, 1);
  memcpy(data, &header, sizeof(header));

  printf ("Answerlen: %d \n", aswrlen);

  unsigned char* p = (unsigned char *) (data + sizeof(header));
  memcpy(p, domain_name, sizeof(domain_name));
  p += sizeof(domain_name);
  memcpy(p, queries.qtype, sizeof(queries.qtype));
  p += sizeof(queries.qtype);
  memcpy(p, queries.qclass, sizeof(queries.qclass));

  // memset(buffer_out, (int) data, aswrlen);
  for(int i = 0; i < aswrlen; i++){
    printf("%0x ", data[i]);
  }
  printf("\n");

  // Sends the read message to the server through the socket
  if( (send(sockfd, buffer_out, strlen(buffer_out), 0)) < 0){
    perror("send");
    close(sockfd);
    return EXIT_FAILURE;
  }

  // Receives an answer from the server
  // int trys = 3, answerlen = 1;
  // while (trys-- > 0){
    recv(sockfd, buffer_in, BUFFER_LEN, 0);
    printf("Server answer: %s\n", buffer_in);
  //}

  close(sockfd);

  return 0;
}
