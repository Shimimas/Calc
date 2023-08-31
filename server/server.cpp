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

    int bind_res = bind(__socket_fd, reinterpret_cast<struct sockaddr *>(&__server_address), sizeof(__server_address));

    if (bind_res < 0) {
        std::cout << "SERVER ERROR: binding connection. Socket has already been establishing." << std::endl;
        close(__socket_fd);
        return ERROR;
    }

    listen(__socket_fd, 1);

    return SUCCES;
}

void Server::exit_code() {
    close(__socket_fd);
    exit(0);
}

void listen_thread(Server& obj) {
    char answer[20];

    std::cout << "Enter the /exit, for end server work" << std::endl;

    do {
        std::cin.getline(answer, 20);
    } while (strcmp(answer, "/exit") != 0);

    obj.exit_code();
}

void Server::work() {
    while (true) {
        __client_fd = accept(__socket_fd, NULL, NULL);

        recv(__client_fd, __buffer, BUFFER_SIZE, 0);
        __calculation();
        send(__client_fd, __buffer, BUFFER_SIZE, 0);

        close(__client_fd);
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