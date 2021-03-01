#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "defs.h"

#define LOG "[Client] "

void receive_data(int client_socket, char buffer[5 * 32], char *parameters[5]){
    printf(LOG"Receiving data.\n");

    int received = 0, length = 5 * 32;
    char *names[] = {
        "Time:",
        "Latitude:",
        "N/S:",
        "Longitude:",
        "E/W:"
    };

    while (received < length){
        received += read(client_socket, buffer + received, length - received);
    }

    for (int i = 0; i < 5; ++i){
        parameters[i] = buffer + i * 32;
        printf("%s %s\n", names[i], parameters[i]);
    }

    printf(LOG"Data received, closing connection.\n");
}

void initialize_client(int *client_socket){
    struct sockaddr_in server_address;
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0){
        printf(ERR_LOG"IP could not be set.\n");
        exit(EXIT_FAILURE);
    }

    printf(LOG"Creating socket.\n");
    if ((*client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf(ERR_LOG"Couldn't create socket.\n");
        exit(EXIT_FAILURE);
    }

    printf(LOG"Connecting to 127.0.0.1:%d.\n", PORT);
    if (connect(*client_socket, (struct sockaddr *)&server_address, sizeof(server_address))){
        printf(ERR_LOG"Unable connect to server.\n");
        exit(EXIT_FAILURE);
    }
}

void client_process(){
    char buffer[5 * 32];
    char *parameters[5];
    
    int client_socket;
    initialize_client(&client_socket);

    receive_data(client_socket, buffer, parameters);
    
    close(client_socket);
}