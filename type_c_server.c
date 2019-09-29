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
#include <time.h>

#define PORTA_SOCKET 5000 // Porta de conexão do socket
#define N_MAX_CLIENTS 5 // Número máximo de clientes

int buffer_server = 0;

void *calc_horas(void *arg)
{
    char buffer_resposta[256];
    char buffer_solicitacao[256];
    int sockEntrada = *(int *)arg;

    while (1)
    {
        //Le o que vem do cliente
        read(sockEntrada, buffer_solicitacao, sizeof(buffer_solicitacao));
        if (strcmp(buffer_solicitacao, "sair") != 0)
        {
            printf("%s\n", "Resposta enviada!");
            const time_t timer = time(NULL);
            strcpy(buffer_resposta, ctime(&timer));

            send(sockEntrada, buffer_resposta, sizeof(buffer_resposta),0);
        }
        else
        {

            close(sockEntrada);
            pthread_exit((void *)0);
        }
    }
}


int config_server_socket()
{
 
    int sockfd;
    struct sockaddr_in serverAddr;
    //Cria o socket
    if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        printf("Erro no Socket!\n");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    //Seta os IPS 
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //Define a porta
    serverAddr.sin_port = htons(PORTA_SOCKET);
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Erro no Socket!\n");
        exit(1);
    }
    //Fica na escuta de ate n clientes
    if (listen(sockfd, N_MAX_CLIENTS) < 0)
    {
        printf("Erro no Socket!\n");
        exit(1);
    }
    return sockfd;
}

void *receber_mensagens(void *arg)
{

    int *sockfd = arg;

    while (1)
    {

        struct sockaddr_in clientAddr;
        //tamanho da estrutura
        unsigned int clntLen;
        clntLen = sizeof(clientAddr);
        pthread_t thread;

        //Fica no aguardo da conexao do cliente

        int client_socked = accept(*sockfd, (struct sockaddr *)&clientAddr, &clntLen);

        if (client_socked < 0)
        {
            printf("Erro no Socket!\n");
            exit(1);
        }
        else
        {
            printf("Conexão bem sucedida !\n");
            
        }

        //Cria uma Thread para cada cliente
        if (pthread_create(&thread, NULL, calc_horas, &client_socked) != 0)
        {
            printf("Erro na Thread!\n");
            exit(1);
        }

        pthread_detach(thread);
    }
}

void main()
{

    pthread_t thread_receber;
    struct sockaddr_in serverAddr;
    int sockfd = config_server_socket();

    pthread_create(&thread_receber, NULL, receber_mensagens, (void *)&sockfd);
    pthread_join(thread_receber, NULL);

   
}
