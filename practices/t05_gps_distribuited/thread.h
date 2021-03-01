/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

#include "defs.h"

void blocking(int opcode, Coordinate *c);
void *uart_reading(void *args);
void launch_uart_thread();
