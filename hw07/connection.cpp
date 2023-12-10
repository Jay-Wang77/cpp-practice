#include "connection.h"
#include <vector>
namespace net{

    [[nodiscard]] ssize_t send(int fd, std::span<const char> data) {
        return ::send(fd, data.data(), data.size(), 0);
    }
    [[nodiscard]] ssize_t receive(int fd, std::span<char> buf) {
        return ::recv(fd, buf.data(), buf.size(), 0);
    }

    Connection::Connection(FileDescriptor&& fd) : fd_(std::move(fd)) {}
    void Connection::send(std::string_view data) const {
        if (::send(fd_.unwrap(), data.data(), data.size(), 0) < 0) {
            throw std::runtime_error("Send failed");
        }
    }

    void Connection::send(std::istream& data) const {
        std::vector<char> buffer(128);
        while (data.read(buffer.data(), buffer.size())) {
            if (::send(fd_.unwrap(), buffer.data(), data.gcount(), 0) < 0) {
                throw std::runtime_error("Send failed");
            }
        }
    }

    [[nodiscard]] ssize_t Connection::receive(std::ostream& stream) const {
        std::vector<char> buffer(128);
        ssize_t n = ::recv(fd_.unwrap(), buffer.data(), buffer.size(), 0);
        if (n > 0) {
            stream.write(buffer.data(), n);
        }
        return n;
    }

    [[maybe_unused]] ssize_t Connection::receive_all(std::ostream& stream) const {
        std::vector<char> buffer(128);
        ssize_t total_received = 0;
        ssize_t n;
        while ((n = ::recv(fd_.unwrap(), buffer.data(), buffer.size(), 0)) > 0) {
            stream.write(buffer.data(), n);
            total_received += n;
        }
        return total_received;
    }

    int Connection::fd() const {
        return fd_.unwrap();
    }
}
