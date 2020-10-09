# Programação de Sockets

**Fundamentos de Redes de Computadores - Trabalho 01 / 2020-1**  
**Professor:** Tiago Alves  
**Alunos:**  
Ailamar Alves Guimarães - 160022673  
Matheus Amaral Moreira  - 170080307

## Introdução  

Esse documento tem o intuido de informar e descrever as atividades feitas no decorrer do Trabalho 01 de FRC. Nesse projeto foi desenvolvido um SOCKET

## Sistema operacional

De acordo como o que foi passado no roteiro do trabalho, concluímos que o modo mais eficaz, baseado no conhecimento da dupla, seria fazer o projeto em Linguagem C com as bibliotecas do GNU/Linux, no caso o Ubuntu. Porém, com algumas limitações de um dos integrantes, foi necessário utilizar o Windows 10 também.

## Ambiente de desenvolvimento

Para o desenvolvimento do projeto foram utilizadas as ferramentas listadas na tabela seguinte:

|    |    |
 ----|:---
**GitHub** | Essa plataforma foi utilizada para armazenamento, integração e controle de versões trabalho.
**Discord** | O Discord foi útil para a dupla realizar o pareamento durante a programação, através de chamadas devido ao afastamento social já que os integrantes não poderiam se encontrar.
**WhatsApp** | Para cominicação mais rápida, marcar horário para reunir e passar qualquer informação que aparecer sobre o projeto.
**Visual Studio Code / CLion**| IDE's utilizadas no desenvolvimento da documentação e código.
**Wireshark** | Usado para analisar e entender os tráfegos de rede e a organização dos protocolos.

## Construção da aplicação

O desenvolvimento da aplicação consistiu nos seguintes desafios:
* Construção e formatação dos dados DNS MX segundo a RFC 1035, Page: 25;
    * Header: ID (2 bytes); Flags (2 bytes); Questions Count (2 bytes); Answer Count (2 bytes); Authority Count (2 bytes); Additional Count (2 bytes).
    * Question: Question Name (tamanho da mensagem + 1 bytes); Question Type (2 bytes); Question Class (2 bytes).
    * Answer: Name (2 bytes); Type (2 bytes); Class (2 bytes); Time To Live (4 bytes); Data Length (2 bytes); Preference (2 bytes); MX Data (Data Length - 2 bytes). 
* Estabelecer conexão com o socket, o envio <sendto()> e a recepção <recvfrom()> usando o protocolo UPD;
* Fazer um algoritmo de tradução dos dados para o formato de envio;
* Fazer um algoritmo de tradução dos dados do formato recebido para o formato legível;
* Responder segundo as expecificações `unb.br <> unb-br.mail.protection.outlook.com`

## Como executar a aplicação

Clonar o repositório:

```
git clone https://github.com/ailamaralves/FRC-ProtocoloDNS.git
```

Executar:

```
gcc main.c -o
```  

Rodar:

```
./main unb.br 8.8.8.8
```

## Instruções de uso

## Limitações

Algumas dificuldades foram encontradas no decorrer do projeto, já que era uma área nova de produção para ambos da dupla.    
Inicialmente foi necessário estudar os documentos disponibilizados pelo professor e fazer algumas pesquisas para se familiarizar melhor com o assunto.  
Outra questão que nos limitou durante o desenvolvimento foi o fato de um dos membros ter tido problema em sua máquina com o GNU/Linux, dificultando na hora de testar e rodar o projeto mas ainda assim com apenas uma máquina para executar deu certo.

## Referências

- DNS: RFC 1034. Disponível em: <https://aprender3.unb.br/pluginfile.php/235919/mod_resource/content/1/rfc1034.pdf>. Acesso em out. 2020.
- DNS: RFC 1035. Diponível em: <https://aprender3.unb.br/pluginfile.php/235920/mod_resource/content/2/rfc1035.pdf>. Acesso em out. 2020.
- DNS QUERY MESSAGE FORMAT. Disponível em: <http://www.firewall.cx/networking-topics/protocols/domain-name-system-dns/160-protocols-dns-query.html>. Acesso em out. 2020.
- C library. Disponível em <http://www.cplusplus.com/reference/clibrary/>. Acesso em out. 2020.
