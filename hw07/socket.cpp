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
        if (getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &val, &len) == -1) {
            return false;
        }
        return val != 0;
    }
    Socket::Socket() {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            throw std::runtime_error("Socket creation failed");
        }
        int opt = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
            close(sock);
            throw std::runtime_error("Setsockopt SO_REUSEADDR failed");
        }
        fd_ = FileDescriptor(sock);
        //use constructor func. create an object of FileD.
        //then pass int socketFd to consF.
    }

    void Socket::listen(uint16_t port) const {
        sockaddr_in addr {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(fd_.unwrap(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
            throw std::runtime_error("Bind failed");
        }

        if (::listen(fd_.unwrap(), SOMAXCONN) == -1) {
            throw std::runtime_error("Listen failed");
        }
    }

    Connection Socket::accept() const {
        if (!is_listening(fd_.unwrap())) {
            throw std::runtime_error("Socket is not listening");
        }

        int client_fd = ::accept(fd_.unwrap(), nullptr, nullptr);
        if (client_fd == -1) {
            throw std::runtime_error("Accept failed");
        }

        return Connection(FileDescriptor(client_fd));
    }

    Connection Socket::connect(std::string destination, uint16_t port) {
        in_addr dest_addr;
        if (destination == "localhost") {
            dest_addr.s_addr = inet_addr("127.0.0.1");
        } else {
            dest_addr.s_addr = inet_addr(destination.c_str());
            if (dest_addr.s_addr == INADDR_NONE) {
                hostent* host = gethostbyname(destination.c_str());
                if (host == nullptr) {
                    throw std::runtime_error("Unknown host");
                }
                dest_addr = *reinterpret_cast<in_addr*>(host->h_addr);
            }
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr = dest_addr;

        if (::connect(fd_.unwrap(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
            throw std::runtime_error("Connect failed");
        }

        return Connection(std::move(fd_));
    }

    Connection Socket::connect(uint16_t port) {
        return connect("localhost", port);
    }

    int Socket::fd() const {
        return fd_.unwrap();
    }
}