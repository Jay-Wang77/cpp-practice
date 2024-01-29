#pragma once

#include <chrono>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <mutex>
#include "logger.h"
namespace fs = std::filesystem;
/**
 * The File Monitor class keeps a record of files in the targetPath and their last modification time
 * Once the monitor has been started, it will continue checking every interval for new, modified and deleted files
 * until it is stopped
 */
class FileMonitor {
public:
    FileMonitor(const std::string &targetpath,
                std::chrono::milliseconds interval = std::chrono::milliseconds(1'000),
                const std::string &logfile = "log.txt");
    std::unordered_map<std::string, fs::file_time_type> fileTimestamps;
    /**
     * Run the monitor.
     * Check the directory every `interval` until `timeout`.
     * Log file changes using the logger.
     */
    void start(std::chrono::seconds timeout = std::chrono::minutes{1});

private:
    mutable Logger logger;
    std::chrono::duration<int, std::milli> interval;
    // TODO: Do you need anything here?
    std::string targetPath;
    std::mutex monitorMutex;
};
