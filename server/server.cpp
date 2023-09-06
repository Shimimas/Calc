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

    int bind_res = bind(__socket_fd, reinterpret_cast<struct sockaddr *>(&__server_address), sizeof(__server_address));

    if (bind_res < 0) {
        std::cout << "SERVER ERROR: binding connection. Socket has already been establishing." << std::endl;
        close(__socket_fd);
        return ERROR;
    }
    
    listen(__socket_fd, 1);

    return SUCCES;
}

int Server::init_epoll() {
    __epfd = epoll_create(EPOLL_START_SIZE);

    if (__epfd < 0) {
        std::cout << "SERVER ERROR: epoll_create" << std::endl;
        close(__socket_fd);
        return ERROR;
    }

    return SUCCES;
}

void Server::exit_code() {
    __exit_code = false;
    close(__socket_fd);
    exit(0);
}

void listen_thread(Server& obj) {
    std::string answer;

    std::cout << "Enter the /exit, for end server work" << std::endl;

    do {
        std::getline(std::cin, answer);
    } while (answer != "/exit");

    obj.exit_code();
}

void accept_thread(Server& obj) {
    int client_fd;
    struct epoll_event ev;

    while (obj.getExitCode()) {
        client_fd = accept(obj.getSocket(), NULL, NULL);

        if (client_fd < 0) {
            std::cout << "accept: ERROR" << std::endl;
            obj.exit_code();
            continue;
        }

        ev.events = EPOLLIN;
        ev.data.fd = client_fd;

        obj.mutex_lock();

        if (epoll_ctl(obj.getEpollFd(), EPOLL_CTL_ADD, client_fd, &ev ) == -1) {
            std::cout << "epoll_ctl: ERROR" << std::endl;
            close(client_fd);
            obj.exit_code();
        }

        obj.mutex_unlock();
    }
}

void Server::work() {
    int activeClientsAmount;

    while (__exit_code) {
        activeClientsAmount = epoll_wait(__epfd, __evlist, MAX_EVENTS, -1);

        __mutex.lock();

        if (activeClientsAmount == 0) {
            continue;
        }

        if (activeClientsAmount == -1) {
            std::cout << "SERVER ERROR: epoll_wait" << std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < activeClientsAmount; i++) {
            int bytes_read = recv(__evlist[i].data.fd, const_cast<char *> (__buffer.data()), BUFFER_SIZE, 0);

            if (bytes_read <= 0 || __buffer == "/exit") {
                epoll_ctl (__epfd, EPOLL_CTL_DEL, __evlist[i].data.fd, &__evlist[i]);
                continue;
            }

            __calculation();

            send(__evlist[i].data.fd, const_cast<char *> (__buffer.data()), BUFFER_SIZE, 0);
        }

        __mutex.unlock();
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
                    __buffer = "Error: devide by zero\0";
                } else {
                    __buffer = std::to_string(a / b);
                }
                break;
            case '*':
                __buffer = std::to_string(a * b);
                break;
            default:
                __buffer = "Error: Bad operator\0";
                break;
        }
    } else {
        __buffer = "Error: bad variables\0";
    }
};

int Server::getSocket() {
    return __socket_fd;
}

int Server::getExitCode() {
    return __exit_code;
}

int Server::getEpollFd() {
    return __epfd;
}

void Server::mutex_lock() {
    __mutex.lock();
}

void Server::mutex_unlock() {
    __mutex.unlock();
}