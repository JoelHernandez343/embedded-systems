/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


#include "defs.h"
#include "gps.h"

extern Coordinate data;
extern pthread_mutex_t lock;
extern pthread_t thread;
extern int exit_flag;

void blocking(int opcode, Coordinate *c){
    pthread_mutex_lock(&lock);

    if (opcode == GET_DATA){
        copy_coordinate(&data, c);
    } else {
        copy_coordinate(c, &data);
    }

    pthread_mutex_unlock(&lock);
}


void *uart_reading(void *args){
    int serial = config_serial("/dev/ttyS0", B9600);
    printf("[SERVER] Openned serial with f %ds\n", serial);

    Coordinate coordinate;
    while (exit_flag) {
        read_coordinate(&coordinate, serial);
        blocking(SET_DATA, &coordinate);
    }

    return NULL;
}

void launch_uart_thread(){
    pthread_create(&thread, NULL, uart_reading, NULL);
}