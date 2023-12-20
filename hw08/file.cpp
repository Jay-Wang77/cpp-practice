#include "file.h"

#include "filesystem.h"

size_t File::get_size() const { return this->content.get_size(); }

bool File::rename(std::string_view new_name) {
  // TODO: Check that a filesystem actually exists, then rename it in the filesystem
  auto fs = filesystem.lock();
  if (!fs || new_name.empty()) {
      return false;
  }

  return fs->rename_file(this->name, new_name);
  return false;
}

const std::string &File::get_name() const { return this->name; }

const FileContent &File::get_content() const { return this->content; }

// TODO file constructor
// the std::string must not
// be copied, but moved into the other object.
File::File(FileContent &&content, std::string_view name) : content(std::move(content)), name(name){}