#include <string.h>

int analize_line(char *line, char **parameters){
    if (strcmp(strtok(line, ","), "$GPGGA")){
        return 0;
    }

    for (int i = 0; i < 5; ++i){
        strcpy(parameters[i], strtok(NULL, ","));
    }

    return 1;
}