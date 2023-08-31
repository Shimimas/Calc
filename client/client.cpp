#include "client.h"

Client::Client() {
    __server_address.sin_port = htons(DEFAULT_PORT);
    __server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &__server_address.sin_addr);

    __buffer.resize(BUFFER_SIZE);
}

void Client::__make_connection() {
    __socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (__socket_fd < 0) {
        std::cout << "CLIENT ERROR: establishing socket error." << std::endl;
        exit(ERROR);
    }

    if (connect(__socket_fd, reinterpret_cast<const struct sockaddr *>(&__server_address), sizeof(__server_address)) < 0) {
        std::cout << "CLIENT ERROR: bad connect." << std::endl;
        close(__socket_fd);
        exit(ERROR);
    }
}

void Client::work() {
    std::cout << "You can send messages in the format (a + b, a - b, a * b, a / b)\n/exit to close connection" << std::endl;

    bool flag = true;

    __make_connection();

    while (flag) {
        std::getline(std::cin, __buffer);

        if (__buffer == "/exit") {
            flag = false;
            close(__socket_fd);
            continue;
        }

        send(__socket_fd, const_cast <char *> (__buffer.data()), BUFFER_SIZE, 0);
        __buffer.resize(BUFFER_SIZE);
        recv(__socket_fd, const_cast <char *> (__buffer.data()), BUFFER_SIZE, 0);
        std::cout << __buffer << std::endl;

    }
    
    close(__socket_fd);
}