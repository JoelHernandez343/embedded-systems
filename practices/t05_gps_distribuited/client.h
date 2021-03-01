/**
 * Joel Hernández @ 2020
 * Github: https://github.com/JoelHernandez343
 * 
 */

void client_process();
void initialize_client(int *client_socket);
void receive_data(int client_socket, char buffer[5 * 32], char *parameters[5]);