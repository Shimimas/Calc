#include "client.h"

int main() {
    Client client;

    if (client.init_socket() == ERROR) {
        return ERROR;
    }

    client.work();
    
    client.socket_close();
    return 0;
}