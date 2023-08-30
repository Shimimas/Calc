#include "client.h"

Client::Client() {
    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);
}

int Client::init_socket() {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        std::cout << "CLIENT ERROR: establishing socket error." << std::endl;
        return ERROR;
    }

    if (connect(socket_fd, reinterpret_cast<const struct sockaddr *>(&server_address), sizeof(server_address)) < 0) {
        std::cout << "CLIENT ERROR: bad connect." << std::endl;
        socket_close();
        return ERROR;
    }
    
    recv(socket_fd, buffer, BUFFER_SIZE, 0);
    std::cout << buffer << std::endl;

    return SUCCES;
}

void Client::work() {
    bool flag = true;
    while (flag) {
        std::cin.getline(buffer, BUFFER_SIZE);
        send(socket_fd, buffer, BUFFER_SIZE, 0);
        if (strcmp(buffer, "/exit") == 0) {
            flag = false;
            continue;
        }
        recv(socket_fd, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
    }
}

void Client::socket_close() {
    close(socket_fd);
}