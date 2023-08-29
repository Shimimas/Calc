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
        int socket_fd;
        int client_fd;
        struct sockaddr_in server_address;
        char buffer[BUFFER_SIZE];

    public:
        Client();
        ~Client() = default;

        void socket_close();
        int init_socket();
        void work();
};