/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "client.h"
#include "server.h"

void check_arguments(int argc, char *argv[]){
    if (argc != 2){
        printf(ERR_LOG"Argument missing.\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < strlen(argv[1]); ++i){
        argv[1][i] = tolower(argv[1][i]);
    }

    if (strcmp(argv[1], "server") && strcmp(argv[1], "client")){
        printf(ERR_LOG"Argument must be \"server\" or \"client\".\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    check_arguments(argc, argv);
    
    if (!strcmp(argv[1], "server")){
        server_process();
    }
    else {
        client_process();
    }

    return 0;
}

