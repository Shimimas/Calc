#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define ERROR -1
#define SUCCES 1
#define DEFAULT_PORT 1601
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

class Client {
    private:
        int __socket_fd;
        int __client_fd;
        struct sockaddr_in __server_address;
        char __buffer[BUFFER_SIZE];

        void __make_connection();
    public:
        Client();
        ~Client() = default;

        void work();
};