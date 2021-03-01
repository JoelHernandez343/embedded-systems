/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "defs.h"

int config_serial(char *serial_device, speed_t baudios){
    struct termios newtermios;
    int fd;

    fd = open(serial_device, (O_RDWR | O_NOCTTY) & ~O_NONBLOCK);

    if (fd == -1){
        printf("Error al abrir el dispositivo tty \n");
        exit( EXIT_FAILURE );
    }

    newtermios.c_cflag = CBAUD | CS8 | CLOCAL | CREAD;
    newtermios.c_iflag = IGNPAR;
    newtermios.c_oflag = 0;
    newtermios.c_lflag = TCIOFLUSH | ~ICANON;
    newtermios.c_cc[VMIN] = 1;
    newtermios.c_cc[VTIME] = 0;


    if (cfsetospeed( &newtermios, baudios ) == -1 ){
        printf("Error al establecer velocidad de salida \n");
        exit( EXIT_FAILURE );
    }

    if (cfsetispeed( &newtermios, baudios ) == -1){
        printf("Error al establecer velocidad de entrada \n");
        exit( EXIT_FAILURE );
    }

    if (tcflush( fd, TCIFLUSH ) == -1){
        printf("Error al limpiar el buffer de entrada \n");
        exit( EXIT_FAILURE );
    }

    if (tcflush( fd, TCOFLUSH ) == -1){
        printf("Error al limpiar el buffer de salida \n");
        exit( EXIT_FAILURE );
    }

    if (tcsetattr( fd, TCSANOW, &newtermios ) == -1){
        printf("Error al establecer los parametros de la terminal \n");
        exit( EXIT_FAILURE );
    }

    return fd;
}

void read_serial_line(char line[128], int serial){
    int index = 0;
    do {
        read(serial, &line[index], 1);
    } while(line[index++] != '\n');

    line[index] = 0;
}

int analize_line(char *line, Coordinate *coordinate){
    int indx = 0, i = 0;
    char word[] = "\0\0\0\0\0\0\0";

    while (line[indx++] != ','){
        word[i++] = line[indx - 1];
    }

    if (strcmp(word, "$GPGGA")){
        return 0;
    }

    printf("LINE: %s\n", line);
    
    i = 0;
    while (line[indx++] != ','){
        coordinate->time[i++] = line[indx - 1];
    }
    coordinate->time[i] = 0;

    i = 0;
    while (line[indx++] != ','){
        coordinate->latitude[i++] = line[indx - 1];
    }
    coordinate->latitude[i] = 0;

    i = 0;
    while (line[indx++] != ','){
        coordinate->ns[i++] = line[indx - 1];
    }
    coordinate->ns[i] = 0;

    i = 0;
    while (line[indx++] != ','){
        coordinate->longitude[i++] = line[indx - 1];
    }
    coordinate->longitude[i] = 0;

    i = 0;
    while (line[indx++] != ','){
        coordinate->ew[i++] = line[indx - 1];
    }
    coordinate->ew[i] = 0;

    return 1;
}

void show_coordinates(Coordinate *coordinate){
    printf("Time: %s\n", coordinate->time);
    printf("Latitude: %s\n", coordinate->latitude);
    printf("N/S: %s\n", coordinate->ns);
    printf("Longitude: %s\n", coordinate->longitude);
    printf("E/W: %s\n", coordinate->ew);
}

void read_coordinate(Coordinate *coordinate, int serial){
    char line[128];
    memset(line, '\0', 128);
    
    do {
        read_serial_line(line, serial);
    } while (analize_line(line, coordinate) == 0);

    show_coordinates(coordinate); 
}

void copy_coordinate(Coordinate *origin, Coordinate *dest){
    strcpy(dest->time, origin->time);
    strcpy(dest->latitude, origin->latitude);
    strcpy(dest->ns, origin->ns);
    strcpy(dest->longitude, origin->longitude);
    strcpy(dest->ew, origin->ew);
}
