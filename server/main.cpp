#include "server.h"

int main() {
    Server server;

    if (server.init_socket() == ERROR) {
        return ERROR;
    }

    server.work();

    server.socket_close();
    return 0;
}