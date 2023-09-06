#include "server.h"

int main() {
    Server server;

    if (server.init_socket() == ERROR) {
        return ERROR;
    }

    if (server.init_epoll() == ERROR) {
        return ERROR;
    }

    std::thread thread1(listen_thread, std::ref(server));
    std::thread thread2(accept_thread, std::ref(server));

    server.work();

    thread1.join();
    thread2.join();
    
    return 0;
}