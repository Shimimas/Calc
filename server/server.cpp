#include "server.h"

Server::Server() {
    __server_address.sin_port = htons(DEFAULT_PORT);
    __server_address.sin_family = AF_INET;
    __server_address.sin_addr.s_addr = htons(INADDR_ANY);
}

int Server::init_socket() {
    __socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (__socket_fd < 0) {
        std::cout << "SERVER ERROR: establishing socket error." << std::endl;
        return ERROR;
    }

    std::cout << "=> Client socket created." << std::endl;

    int bind_res = bind(__socket_fd, reinterpret_cast<struct sockaddr *>(&__server_address), sizeof(__server_address));

    if (bind_res < 0) {
        std::cout << "SERVER ERROR: binding connection. Socket has already been establishing." << std::endl;
        return ERROR;
    }

    listen(__socket_fd, 1);
    __client_fd = accept(__socket_fd, NULL, NULL);

    strcpy(__buffer, "SERVER: You can send messages in the format (a + b, a - b, a * b, a / b)\n/exit to close connection");
    send(__client_fd, __buffer, BUFFER_SIZE, 0);

    return SUCCES;
}

void Server::socket_close() {
    close(__socket_fd);
}

void Server::work() {
    bool flag = true;
    while (flag) {
        recv(__client_fd, __buffer, BUFFER_SIZE, 0);
        if (strcmp(__buffer, "/exit") == 0) {
            flag = false;
            continue;
        }
        __calculation();
        send(__client_fd, __buffer, BUFFER_SIZE, 0);
    }
}

bool Server::__isNumeric(std::string const &str) {
    return std::regex_match(str, std::regex("^[-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?$"));
}

void Server::__calculation() {
    std::string parsing_string(__buffer);
    std::regex regex(" ");
 
    std::vector<std::string> out(
                    std::sregex_token_iterator(parsing_string.begin(), parsing_string.end(), regex, -1),
                    std::sregex_token_iterator()
                    );
    if (__isNumeric(out[0]) && __isNumeric(out[2])) {
        double a = std::stod(out[0]);
        double b = std::stod(out[2]);

        switch (out[1][0]) {
            case '+':
                strcpy(__buffer, std::to_string(a + b).data());
                break;
            case '-':
                strcpy(__buffer, std::to_string(a - b).data());
                break;
            case '/':
                if (std::stod(out[2]) == 0) {
                    strcpy(__buffer, "Error: devide by zero");
                } else {
                    strcpy(__buffer, std::to_string(a / b).data());
                }
                break;
            case '*':
                strcpy(__buffer, std::to_string(a * b).data());
                break;
            default:
                strcpy(__buffer, "Error: Bad operator");
                break;
        }
    } else {
        strcpy(__buffer, "Error: bad variables");
    }
};