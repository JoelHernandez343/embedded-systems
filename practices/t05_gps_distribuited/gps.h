/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

#include <termios.h>

#include "defs.h"

int config_serial(char *serial_device, speed_t baudios);
void read_serial_line(unsigned char line[128], int serial);
void read_coordinate(Coordinate *coordinates, int serial);
void copy_coordinate(Coordinate *origin, Coordinate *dest);