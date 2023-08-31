#include "server.h"

int main() {
    Server server;

    if (server.init_socket() == ERROR) {
        return ERROR;
    }

    std::thread thread(listen_thread, std::ref(server));

    server.work();

    thread.join();
    
    return 0;
}