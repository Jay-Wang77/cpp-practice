#include "filesystem.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

Filesystem::Filesystem() {}

bool Filesystem::register_file(const std::string &name,
                               std::shared_ptr<File> file) {
  // TODO: Do sanity checks first! Is that actually a good name, is the pointer set,
  //       does a file with that name already exists? Anything else to check here?
  // check if pointer for the passed file is null by using !file
  if(name.empty() || !file || files.find(name) != files.end()){
      return false;
  }

  // Creater a shared pointer to the this object
  auto thisptr = this->shared_from_this();

  // TODO: Check if the file already has a filesystem, if so, it may only be
  //       the same as this one, not another one!
  // use shared_ptr object visit member of filesystem
  if (auto existingFs = file->filesystem.lock()) {
      if (existingFs.get() != this) {
          return false;
      }
  }
  // because file->name is empty str, must be updated so that corresponds
  // the name registered in filesystem!!
  file->name = name;
  file->filesystem = std::move(thisptr);

  // TODO: Now actually store it in the filesystem
  files[name] = file;
  return true;
}

bool Filesystem::remove_file(std::string_view name) {
  // TODO: Remove file from the filesystem, return true if successful, false otherwhise.
  //       false may indicate multple different reasons for failure, which is not impotant
  std::string nameStr(name);
  if(name.empty() || files.find(nameStr) == files.end()){
        return false;
    }
    files.erase(files.find(nameStr));
  return true;
}

bool Filesystem::rename_file(std::string_view source, std::string_view dest) {
  // TODO: Check filesystem for the source and dest files, if it's possible to rename
  //       the source file, then update the filesystem
  if (source.empty() || dest.empty() || source == dest) {
      return false;
  }
  auto sourceIt = files.find(std::string(source));
  if (sourceIt == files.end() || files.find(std::string(dest)) != files.end()) {
      return false;
  }
  auto file = sourceIt->second;
  files.erase(sourceIt);
  file->name = dest;
  files[std::string(dest)] = file;
  return true;
}

std::shared_ptr<File> Filesystem::get_file(std::string_view name) const {
  // TODO: Check the filesystem for the given file, return it if you found it,
  //       else just return nothing
  auto it = files.find(std::string(name));
  if (it != files.end()) {
      return it->second;
  }
  return nullptr;
}

size_t Filesystem::get_file_count() const {
  // TODO: how many files have you got?
  return files.size();
}

size_t Filesystem::in_use() const {
  // TODO: sum up all real file sizes
  size_t total_size = 0;
  for(const auto &pair: files){
      total_size += pair.second->get_size();
  }
  return total_size;
}

// convenience function so you can see what files are stored
std::string Filesystem::file_overview(bool sort_by_size) {
  std::ostringstream output;
  // this function is not tested, but it may help you when debugging.

  output << "files in filesystem: " << std::endl;

  for (auto&& entry : this->files) {
      // TODO: fix printing name, type and size
      output << entry.second->get_name() << std::endl;
      output << entry.second->get_type() << std::endl;
      output << entry.second->get_size() << std::endl;
  }
  return std::move(output).str();
}

std::vector<std::shared_ptr<File>>
Filesystem::files_in_size_range(size_t max, size_t min) const {
  // TODO: find the matching files and return them
  std::vector<std::shared_ptr<File>> file;
  for (const auto& pair : files) {
      size_t fileSize = pair.second->get_size();
      if (fileSize >= min && fileSize <= max) {
          file.push_back(pair.second);
      }
  }
  return file;
}
