#include "server.h"

Server::Server() {
    __server_address.sin_port = htons(DEFAULT_PORT);
    __server_address.sin_family = AF_INET;
    __server_address.sin_addr.s_addr = htons(INADDR_ANY);

    __buffer.resize(BUFFER_SIZE);
}

int Server::init_socket() {
    __socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (__socket_fd < 0) {
        std::cout << "SERVER ERROR: establishing socket error." << std::endl;
        return ERROR;
    }

    fcntl(__socket_fd, F_SETFL, O_NONBLOCK);

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

void Server::__select_inizialize() {
    FD_ZERO(&__readset);
    FD_SET(__socket_fd, &__readset);

    for(std::set<int>::iterator it = __clients.begin(); it != __clients.end(); it++) {
        FD_SET(*it, &__readset);
    }

    __timeout.tv_sec = 15;
    __timeout.tv_usec = 0;
}

void Server::__accept_check() {
    if (FD_ISSET(__socket_fd, &__readset)) {
        int sock = accept(__socket_fd, NULL, NULL);

        if (sock < 0) {
            std::cout << "accept: ERROR" << std::endl;
            exit(3);
        }
        
        fcntl(sock, F_SETFL, O_NONBLOCK);
        
        __clients.insert(sock);
    }
}

void Server::__data_working() {
    for(std::set<int>::iterator it = __clients.begin(); it != __clients.end(); it++) {
        if (FD_ISSET(*it, &__readset)) {
            int bytes_read = recv(*it, const_cast<char *> (__buffer.data()), BUFFER_SIZE, 0);

            if (bytes_read <= 0 || __buffer == "/exit") {
                close(*it);
                __delayed_deletion.push(it);
                continue;
            }

            __calculation();
            send(*it, const_cast<char *> (__buffer.data()), BUFFER_SIZE, 0);
        }
    }
}

void Server::__clear_delete_clients() {
    while (!__delayed_deletion.empty()) {
        __clients.erase(*(__delayed_deletion.front()));
        __delayed_deletion.pop();
    }
}

void Server::work() {
    while (true) {
        __select_inizialize();

        int mx = std::max(__socket_fd, *(std::max_element(__clients.begin(), __clients.end())));

        if (select(mx + 1, &__readset, NULL, NULL, &__timeout) <= 0) {
            continue;
        }

        __accept_check();
        __data_working();
        __clear_delete_clients();
    }
}

bool Server::__isNumeric(std::string const &str) {
    return std::regex_match(str, std::regex("^[-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?$"));
}

void Server::__calculation() {
    std::string parsing_string(const_cast<char *> (__buffer.data()));
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
                __buffer = std::to_string(a + b);
                break;
            case '-':
                __buffer = std::to_string(a - b);
                break;
            case '/':
                if (std::stod(out[2]) == 0) {
                    __buffer = std::string("Error: devide by zero");
                } else {
                    __buffer = std::to_string(a / b);
                }
                break;
            case '*':
                __buffer = std::to_string(a * b);
                break;
            default:
                __buffer = std::string("Error: Bad operator");
                break;
        }
    } else {
        __buffer = std::string("Error: bad variables");
    }
};