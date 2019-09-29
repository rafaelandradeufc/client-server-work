#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <ctype.h>

#define PORTA_SOCKET 5000 // Porta de conexão do socket
#define IP_SOCKET_SERVIDOR "127.0.0.1" // Endereço de IP do servidor se for local
//#define IP_SOCKET_SERVIDOR "192.168.0.101" // Endereço de IP do servidor se for externo

char buffer_client[256];

void *iniciar(void *arg)
{
  

    printf("Thread 1: terminal_cliente executando....\n");

    printf("Digite 'hora' ou 'sair'!\n-> ");
    do
    {
        scanf("%s", buffer_client);
        fflush(stdin);

    } while (strcmp(buffer_client, "sair") != 0);

    printf("Thread 1: terminal_cliente finalizado!\n");
}

void *enviar_receber(void *arg)
{
    int *clienteSockfd = arg;
    char buffer_resposta[256];

    printf("Thread 2: envia_recebe iniciou!\n");

    while (1)
    {

        if (strcmp(buffer_client, "hora") == 0)
        {

            send(*clienteSockfd, buffer_client, sizeof(buffer_client),0);
            read(*clienteSockfd, buffer_resposta, sizeof(buffer_resposta));

            printf("HORAS: %s\n", buffer_resposta);

            strcpy(buffer_client, "");
        }
        else if (strcmp(buffer_client, "sair") == 0)
        {
            close(*clienteSockfd);
            break;
        }
    }

    printf("Thread 2: envia_recebe finalizado!\n");
}

int config_client_socket()
{

    int clienteSockfd;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(serv_addr);

    clienteSockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clienteSockfd < 0)
    {
        printf("Erro no Socket\n");
        exit(1);
    }
  
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //Define o IP do servidor
    serv_addr.sin_addr.s_addr = inet_addr(IP_SOCKET_SERVIDOR);
    //Define a porta de conexao
    serv_addr.sin_port = htons(PORTA_SOCKET);
    //Faz a conexao com o servidor
    if (connect(clienteSockfd, (struct sockaddr *)&serv_addr, addrlen) < 0)
    {
        printf("Erro no Socket\n");
        exit(1);
    }
    return clienteSockfd;
}

void main()
{

    pthread_t thread0, thread1;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(serv_addr);
    int descritorCliente;
    descritorCliente = config_client_socket();

    pthread_create(&thread1, NULL, enviar_receber, (void *)&descritorCliente);
    pthread_create(&thread0, NULL, iniciar,NULL);

    pthread_join(thread0, NULL);
    pthread_join(thread1, NULL);
}
