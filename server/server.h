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
#include <fcntl.h>
#include <sys/epoll.h>
#include <mutex>

#define ERROR -1
#define SUCCES 1
#define DEFAULT_PORT 1601
#define BUFFER_SIZE 1024
#define CLIENT_NAME_SIZE 20
#define EPOLL_START_SIZE 5
#define MAX_EVENTS 1

class Server {
    private:
        int __delete_client = -1;
        int __socket_fd;
        struct sockaddr_in __server_address;
        std::string __buffer;
        int __epfd;
        struct epoll_event __evlist[MAX_EVENTS];
        int __client_socket;
        struct epoll_event __ev;
        bool __exit_code = true;
        std::mutex __mutex;

        void __calculation();
        bool __isNumeric(std::string const &str);

    public:
        Server();
        ~Server() = default;

        int init_socket();
        int init_epoll();
        void create_listen_pthread();
        void work();
        void exit_code();

        int getSocket();
        int getExitCode();
        int getEpollFd();

        void mutex_lock();
        void mutex_unlock();
};

void listen_thread(Server& obj);
void accept_thread(Server& obj);