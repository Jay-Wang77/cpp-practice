#include "client.h"

namespace net{
    Connection Client::connect(const std::string& destination, uint16_t port) {
        return socket.connect(destination, port);
    }

    Connection Client::connect(uint16_t port) {
        return socket.connect(port);
    }
}