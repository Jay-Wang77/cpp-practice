#include "filecontent.h"

// TODO implement constructors
// creat a object of shared_ptr:data, stored the reference of content,
// so original data:content share the actual data with shared_ptr:data
FileContent::FileContent(const std::string &content):data(std::make_shared<std::string> (content)) {}
// creat an object of shared_ptr:data, move actual data from original data:content to shared_ptr:data by move
FileContent::FileContent(std::string &&content):data(std::make_shared<std::string> (std::move(content))){}
FileContent::FileContent(const char *content):data(std::make_shared<std::string> (content)) {}

size_t FileContent::get_size() const{
    return data ? data->size() : 0;
}

std::shared_ptr<const std::string>
FileContent::get() const{
    return data;
}

// TODO implement member functions
