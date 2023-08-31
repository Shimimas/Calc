#pragma once

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <regex>
#include <vector>
#include <csignal>
#include <thread>
#include <set>
#include <fcntl.h>
#include <algorithm>
#include <queue>

#define ERROR -1
#define SUCCES 1
#define DEFAULT_PORT 1601
#define BUFFER_SIZE 1024
#define CLIENT_NAME_SIZE 20

class Server {
    private:
        int __delete_client = -1;
        int __socket_fd;
        int __client_fd;
        struct sockaddr_in __server_address;
        char __buffer[BUFFER_SIZE];
        std::set<int> __clients;
        std::queue<std::set<int>::iterator> __delayed_deletion;
        timeval __timeout;
        fd_set __readset;

        void __calculation();
        bool __isNumeric(std::string const &str);
        void __select_inizialize();
        void __accept_check();
        void __data_working();
        void __clear_delete_clients();

    public:
        Server();
        ~Server() = default;

        int init_socket();
        void create_listen_pthread();
        void work();
        void exit_code();
};

void listen_thread(Server& obj);