#include "server.h"

namespace net{
    Server::Server(uint16_t port) {
        listen_socket_.listen(port);
    }

    Connection Server::accept() {
        return listen_socket_.accept();
    }
}