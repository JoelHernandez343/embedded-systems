#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processing.h"
#include "defs.h"

int get_info(char *file_name, char *buffer){
    FILE *file;
    char *parameters[5];
    char line[128], aux[128];

    memset(buffer, 0, 5 * 32);

    for (int i = 0; i < 5; ++i){
        parameters[i] = buffer + i * 32;
    }

    if ((file = fopen(file_name, "r")) == NULL){
        printf(ERR_LOG"Error opening file %s.\n", file_name);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file)){
        strcpy(aux, line);

        if (analize_line(aux, parameters)){
            fclose(file);
            return 1;
        }
    }

    printf(ERR_LOG"No line was found starting with \"$GPGGA\"\n");
    return 0;
}