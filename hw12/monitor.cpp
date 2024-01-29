#include "monitor.h"
#include <chrono>
#include <thread>
#include <filesystem>
#include <iostream>
#include <unordered_set>
namespace fs = std::filesystem;
FileMonitor::FileMonitor(const std::string &targetpath,
                         std::chrono::milliseconds interval,
                         const std::string &logfilename)
    : logger{logfilename}
    , interval{interval},
      targetPath{targetpath}
{
    for (const auto &entry : fs::directory_iterator(targetPath)) {
        if (fs::is_regular_file(entry.status())) {
            auto fileTime = fs::last_write_time(entry);
            fileTimestamps[entry.path().string()] = fileTime;
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
        std::this_thread::sleep_for(interval);
        std::lock_guard<std::mutex> guard(monitorMutex);
        std::unordered_set<std::string> currentFiles;

        for (const auto &entry : fs::recursive_directory_iterator(targetPath)) {
            std::string path = entry.path().string();
            currentFiles.insert(path);
            if (fs::is_regular_file(entry.status()) || fs::is_directory(entry.status())) {
                auto fileTime = fs::last_write_time(entry);

                auto it = fileTimestamps.find(path);
                if (it == fileTimestamps.end()) {
                    logger.log(path, status::added);
                    fileTimestamps[path] = fileTime;
                } else if (it->second != fileTime) {
                    logger.log(path, status::changed);
                    it->second = fileTime;
                }
            }
        }
        for (auto it = fileTimestamps.begin(); it != fileTimestamps.end();) {
            if (currentFiles.find(it->first) == currentFiles.end()) {
                logger.log(it->first, status::removed);
                it = fileTimestamps.erase(it);
            } else {
                ++it;
            }
        }
    }
}
