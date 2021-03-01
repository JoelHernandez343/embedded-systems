/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "defs.h"

#define LOG "[Client] "

void receive_data(int client_socket, char buffer[5 * 32], char *parameters[5]){
    printf(LOG"Receiving data.\n");

    int received = 0, length = 5 * 32;
    char *names[] = {
        "\033[40;34mTime\033[0m:      ",
        "\033[40;34mLatitude\033[0m:  ",
        "\033[40;34mN/S\033[0m:       ",
        "\033[40;34mLongitude\033[0m: ",
        "\033[40;34mE/W\033[0m:       "
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

int hostname_to_ip(char * hostname , char* ip){
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL){
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++){
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}


void initialize_client(int *client_socket){
    struct sockaddr_in server_address;
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(VPORT);

    // Solo se usa si se conecta de forma remota
    char ip[100];
    hostname_to_ip(SERVER_DOMAIN_NAME, ip);

    if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0){
        printf(ERR_LOG"IP %s could not be set.\n", ip);
        exit(EXIT_FAILURE);
    }

    printf(LOG"Creating socket.\n");
    if ((*client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf(ERR_LOG"Couldn't create socket.\n");
        exit(EXIT_FAILURE);
    }

    printf(LOG"Connecting to "SERVER_IP":%d.\n", VPORT);
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