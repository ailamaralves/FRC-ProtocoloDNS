#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
 
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

struct mail_exchange{
  char* name;
  unsigned char cocat[2];
};

struct answer{
  unsigned short name;
  unsigned short atype;
  unsigned short aclass;
  unsigned int time_to_live;
  unsigned short datalength;
  unsigned short preference;
  struct mail_exchange mailx;
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
  char buffer_in [BUFFER_LEN];
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
  unsigned short rand_id = rand()%65535;  

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

  // Zeroing the buffers
  memset(buffer_in, 0x0, BUFFER_LEN);
  memset(buffer_out, 0x0, BUFFER_LEN);


  unsigned aswrlen = sizeof(header) + (sizeof(domain_name)) +
    sizeof(queries.qtype) + sizeof(queries.qclass);
  unsigned char* data = calloc(aswrlen, 1);
  unsigned char* iterator = (unsigned char *) data;

  memcpy(iterator, &header, sizeof(header));
  iterator += sizeof(header);
  memcpy(iterator, domain_name, sizeof(domain_name));
  iterator += sizeof(domain_name);
  free(domain_name);
  memcpy(iterator, &queries.qtype, sizeof(queries.qtype));
  iterator += sizeof(queries.qtype);
  memcpy(iterator, &queries.qclass, sizeof(queries.qclass));

  // memset(buffer_out, (int) data, aswrlen);
  for(int i = 0; i < aswrlen; i++){
    printf("%0x ", data[i]);
  }
  printf("\n");

  // buffer_out = data;

  // Sends the read message to the server through the socket
  int resp;
  if (resp = sendto(sockfd, data, aswrlen, 0, (struct sockaddr *) &server, (socklen_t) sizeof(server)) == -1){
    perror("send");
    close(sockfd);
    return EXIT_FAILURE;
  }


  free(data);
  sleep(2);

  // Receives an answer from the server
  unsigned int bytes, length;
  bytes = recvfrom (sockfd, buffer_in, BUFFER_LEN, MSG_DONTWAIT, (struct sockaddr *) &server, (socklen_t*)&length);

  printf("Server answer: %d\n", bytes);
  for(int i = 0; i < bytes; i++){
    printf("%0x ", buffer_in[i]);
  }
  printf("\n");

  if(bytes == -1){
    close(sockfd);
    printf("Nao foi possível coletar a entrada MX para %s\n", argv[1]);
    exit(-1);
  }

  if(bytes == 12){
    close(sockfd);
    printf("Dominio %s nao possui entrada MX\n", argv[1]);
    exit(-1);
  }

/*
  a0 24 81 80 00 01 00 01 00 00 00 00 [03 75 6e 62 02 62 72 00] 00 0f 00 01 
  |c0 0c, 00 0f, 00 01, 00 00 34 dd, *00 26*, 00 00, [06 75 6e 62 2d 62 72 04 6d 61 69 6c 0a 70 
  72 6f 74 65 63 74 69 6f 6e 07 6f 75 74 6c 6f 6f 6b 03 63 6f 6d 00]|

  1b fd 81 80 00 01 00 05 00 00 00 00 </>[06 67 6f 6f 67 6c 65 03 63 6f 6d 00] 00 0f 00 01 
  |c0 0c 00 0f 00 01 00 00 01 c3 *00 11* 00 1e [04 61 6c 74 32 <.>05 61 73 70 6d 78 01 6c c0 0c| 
  |c0 0c 00 0f 00 01 00 00 01 c3 *00 09* 00 28 [04 61 6c 74 33 c0 2f| 
  |c0 0c 00 0f 00 01 00 00 01 c3 *00 04* 00 0a [c0 2f|
  |c0 0c 00 0f 00 01 00 00 01 c3 *00 09* 00 32 [04 61 6c 74 34 c0 2f| 
  |c0 0c 00 0f 00 01 00 00 01 c3 *00 09* 00 14 [04 61 6c 74 31 c0 2f|

  * * = numero de bites na frente
  [ ] = ascii
  | | = struct answer
  </> = c0 0c
  <.> = c0 2f
*/
  iterator = (unsigned char*)buffer_in;
  iterator += sizeof(struct dns_header);
  int name_size = 0;

  printf("O iterator é: %0x\n", *(iterator));

  for(; *iterator != 0; name_size++, iterator++) ;

  iterator -= name_size;
  //iterator += sizeof(struct query) - sizeof(char*);
  printf("O iterator é: %0x\n", *(iterator));
  printf("Name size: %d\n", name_size);
  domain_name = calloc(name_size - 1, sizeof(char));

  {
    int i = 0;
    while (1){
      i++;
      for(int atual = --i; i <= iterator[i] + atual && i < name_size; i++){
        printf("i: %d\n", i);
        domain_name[i] = iterator[i+1];
      }
      if (i+1 >= name_size) break;
      domain_name[i-1] = '.';
    }
    domain_name[i] = '\0';
  }

  printf("%s <>", domain_name);
  printf("\n");

  close(sockfd);

  return 0;
}
