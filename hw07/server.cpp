#include "server.h"

namespace net{
    Server::Server(uint16_t port) : socket() {
        socket.listen(port);
    }

    Connection Server::accept() const {
        return socket.accept();
    }
}