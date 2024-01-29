#include "logger.h"
#include <filesystem>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;

Logger::Logger(const std::string &filename) : file{} {
    fs::path logPath(filename);
    fs::path backupPath = logPath;
    backupPath += "_old";
    if (fs::exists(logPath)) {
        fs::rename(logPath, backupPath);
    }
    file.open(logPath);
    if (!file.is_open()) {
        throw std::runtime_error("can not open log file.");
    }
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file << "Commencing logging for directory: "
         << "TODO: insert current path here.."
         << std::endl
         << "Current time is: "
         << std::ctime(&time)
         << std::endl;
}
Logger::~Logger() {
    if (file.is_open()) {
        file.close();
    }
}
void Logger::log(const std::string &path, status what) {
    if (not file.is_open()) {
        throw std::runtime_error("File could not be opened!");
    }

    switch (what) {
    case status::added :
        file << "+ File was added:    " << path << std::endl;
        break;
    case status::changed :
        file << "! File was modified: " << path << std::endl;
        break;
    case status::removed :
        file << "~ File was deleted:  " << path << std::endl;
        break;
    default:
        file << "? Error! Unknown file status detected for: " << path << std::endl;
        break;
    }
}

