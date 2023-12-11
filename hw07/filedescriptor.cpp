#include "filedescriptor.h"
#include <unistd.h>
// TODO Implement the methods of the FileDescriptor class as given in the filedescriptor.h header.

namespace net{
    FileDescriptor::FileDescriptor() : fd_(std::nullopt) {}

    FileDescriptor::FileDescriptor(int fd) : fd_(fd) {}

    FileDescriptor::~FileDescriptor() {
        if (fd_) {
            close(*fd_);
        }
    }
    //once the file descriptor owns the descriptor it will be closed when it goes out of scope.

    FileDescriptor::FileDescriptor(FileDescriptor&& other) :fd_{std::move(other.fd_)}{
        other.fd_.reset();
    }
    //move source from other to current object
    //ensure other no longer has the ownership of resource after movement.
    FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) {
        if(this != &other){
            if(fd_.has_value()){
                close(fd_.value());
            }
            fd_ = std::move(other.fd_);
            other.fd_.reset();
        }
        return *this;
    }

    int FileDescriptor::unwrap() const {
        return fd_.value_or(-1);
    }
}