#include "monitor.h"
#include <chrono>
#include <thread>
#include <filesystem>
#include <iostream>
#include <unordered_set>
std::chrono::system_clock::time_point to_system_clock(const std::filesystem::file_time_type &tp) {
    using namespace std::chrono;
    return system_clock::time_point(duration_cast<system_clock::duration>(tp.time_since_epoch()));
}
FileMonitor::FileMonitor(const std::string &targetpath,
                         std::chrono::milliseconds interval,
                         const std::string &logfilename)
    : logger{logfilename}
    , interval{interval},
      targetPath{targetpath}
{
    for (const auto &entry : std::filesystem::directory_iterator(targetPath)) {
        if (entry.is_regular_file()) {
            filesLastModified[entry.path()] = std::filesystem::last_write_time(entry);
        }
    }
    // TODO: Initialize a mapping for all files in the target path
}

// Monitor the targetpath for changes and pass information to the logger in case of differences
void FileMonitor::start(std::chrono::seconds timeout) {
    // TODO: Monitor the system for the given amount of time.
    // Every given interval, check for modifications to the files and log it
    auto startTime = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - startTime < timeout) {
        std::unordered_map<std::filesystem::path, std::filesystem::file_time_type> currentFiles;
        std::unordered_set<std::filesystem::path> checkedFiles;
        for (const auto &entry : std::filesystem::directory_iterator(targetPath)) {
            auto currentPath = entry.path();
            auto currentLastWriteTime = std::filesystem::last_write_time(entry);
            currentFiles[currentPath] = currentLastWriteTime;
            if (filesLastModified.find(currentPath) == filesLastModified.end()) {
                logger.log(currentPath.string(), status::added);
            } else if (filesLastModified[currentPath] != currentLastWriteTime) {
                logger.log(currentPath.string(), status::changed);
            }
            checkedFiles.insert(currentPath);
        }
        for (const auto &file : filesLastModified) {
            if (checkedFiles.find(file.first) == checkedFiles.end()) {
                logger.log(file.first.string(), status::removed);
            }
        }
        filesLastModified = std::move(currentFiles);
        std::this_thread::sleep_for(interval);
    }
}
