#include "client.h"

namespace net{
    Connection Client::connect(uint16_t port) {
        socket_.connect("localhost", port);
    }
    Connection Client::connect(const std::string& destination, uint16_t port) {
        socket_.connect(destination, port);
    }
}