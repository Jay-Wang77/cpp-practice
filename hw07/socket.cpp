#include "socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <stdexcept>
#include <unistd.h>

namespace net{

    bool is_listening(int fd){
        int val;
        socklen_t len = sizeof(val);
        if (getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &val, &len) == -1){
            return false;
        }
        return true;
    }
    Socket::Socket() {
        int socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if(socketFd < 0){
            throw std::runtime_error("failed to create a socket");
        }
        fd_ = FileDescriptor(socketFd);
        //use constructor func. create an object of FileD.
        //then pass int socketFd to consF.
    }

    void Socket::listen(uint16_t port) const {
        struct sockaddr_in server_addr;
        std::memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if(bind(fd_.unwrap(), (struct sockaddr*) &server_addr, sizeof(server_addr) == -1)){
            throw std::runtime_error("Failed to bind");
        }
        if(::listen(fd_.unwrap(), SOMAXCONN) == -1){
            throw std::runtime_error("Failed to listen");
        }
    }

    Connection Socket::accept() const {
        int new_sockfd = ::accept(fd_.unwrap(), nullptr, nullptr);
        if (new_sockfd < 0) {
            throw std::runtime_error("Failed to accept connection");
        }

        FileDescriptor new_fd(new_sockfd);
        return Connection(std::move(new_fd));
    }

    Connection Socket::connect(std::string destination, uint16_t port) {
        struct sockaddr_in server_addr;
        std::memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (destination == "localhost") {
            destination = "127.0.0.1";
        }

        if (inet_addr(destination.c_str()) != INADDR_NONE) {
            server_addr.sin_addr.s_addr = inet_addr(destination.c_str());
        } else {
            struct hostent* host;
            host = gethostbyname(destination.c_str());
            if (host == NULL) {
                throw std::runtime_error("Failed to resolve hostname");
            }
            std::memcpy(&server_addr.sin_addr, host->h_addr, host->h_length);
        }

        if (::connect(fd_.unwrap(), (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            throw std::runtime_error("Failed to connect to server");
        }
        return Connection(release_fd());
    }

    Connection Socket::connect(uint16_t port) {
        return connect("localhost", port);
    }

    int Socket::fd() const {
        return fd_.unwrap();
    }
}