/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define PORT 4000
#define VPORT 15513
#define SERVER_IP "3.14.182.203"
#define SERVER_DOMAIN_NAME "2.tcp.ngrok.io"

#define ERR_LOG "\033[40;31mERR!\033[0m "
#define DONE_LOG "\033[40;32mDONE\033[0m "

#define GET_DATA 1
#define SET_DATA 2

typedef struct coordinate {
    char time[32];
    char latitude[32];
    char ns[32];
    char longitude[32];
    char ew[32];
} Coordinate;


#endif