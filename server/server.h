#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <regex>
#include <vector>

#define ERROR -1
#define SUCCES 1
#define DEFAULT_PORT 1601
#define BUFFER_SIZE 1024

class Server {
    private:
        int __socket_fd;
        int __client_fd;
        struct sockaddr_in __server_address;
        char __buffer[BUFFER_SIZE];

        void __calculation();
        bool __isNumeric(std::string const &str);
    public:
        Server();
        ~Server() = default;

        void socket_close();
        int init_socket();
        void work();
};