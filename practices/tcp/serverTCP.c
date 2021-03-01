#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PUERTO 5000		//Número de puerto asignado al servidor
#define COLA_CLIENTES 5 //Tamaño de la cola de espera para clientes
#define TAM_BUFFER 100
#define EVER 1

void handler(int rec_signal);
void client_process(int cliente_sockfd);
void initialize_server();
void initialize_signals();

int sockfd;

int main(int argc, char **argv)
{
    pid_t pid;
    int cliente_sockfd;

    initialize_signals();
    initialize_server();

    for (; EVER;)
    {
        printf("En espera de peticiones de conexión ...\n");
        if ((cliente_sockfd = accept(sockfd, NULL, NULL)) < 0)
        {
            perror("Ocurrio algun problema al atender a un cliente\n");
            exit(1);
        }
        pid = fork();
        if (!pid){
            client_process(cliente_sockfd);
        }
    }
    printf("Concluimos la ejecución de la aplicacion Servidor \n");

    close(sockfd);

    return 0;
}

void initialize_server(){
    struct sockaddr_in direccion_servidor;
    memset(&direccion_servidor, 0, sizeof(direccion_servidor)); //se limpia la estructura con ceros
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = htons(PUERTO);
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;

    printf("Creando Socket ....\n");
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Ocurrio un problema en la creacion del socket\n");
        exit(1);
    }

    printf("Configurando socket ...\n");
    if (bind(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0)
    {
        perror("Ocurrio un problema al configurar el socket\n");
        exit(1);
    }

    printf("Estableciendo la aceptacion de clientes...\n");
    if (listen(sockfd, COLA_CLIENTES) < 0)
    {
        perror("Ocurrio un problema al crear la cola de aceptar peticiones de los clientes\n");
        exit(1);
    }
}

void initialize_signals(){
    if (signal(SIGCHLD, handler) == SIG_ERR){
        perror("Error in the handler\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, handler) == SIG_ERR){
        perror("Error in the handler\n");
        exit(EXIT_FAILURE);
    }
}

void client_process(int cliente_sockfd){
    char leer_mensaje[TAM_BUFFER];

    printf("Se aceptó un cliente, atendiendolo \n");
    if (read(cliente_sockfd, leer_mensaje, TAM_BUFFER) < 0)
    {
        perror("Ocurrio algun problema al recibir datos del cliente\n");
        exit(EXIT_FAILURE);
    }
    printf("El cliente nos envio el siguiente mensaje: \n %s \n", leer_mensaje);
    if (write(cliente_sockfd, "Bienvenido cliente", 19) < 0)
    {
        perror("Ocurrio un problema en el envio de un mensaje al cliente\n");
        exit(EXIT_FAILURE);
    }
    close(cliente_sockfd);
    exit(0);
}

void handler(int rec_signal){
    int status;
    pid_t pid;

    if (rec_signal == SIGCHLD){
        printf("Se recibió la señal de terminación de hijo en el servidor\n");
        pid = wait(&status);

        printf("Finished the process w/ pid %d w/ status %d\n", pid, status >> 8);
    }
    else if (rec_signal == SIGINT){
        printf("Se recibió la señal de terminación.\n");
        printf("Concluimos la ejecución de la aplicacion Servidor \n");
        close(sockfd);

        exit(EXIT_SUCCESS);
    }
}