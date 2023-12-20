#include "document.h"

// accepts a para. of rvalue reference type: content
// the ownership of content move to File from original object.
Document::Document(FileContent &&content) : File{std::move(content)} {}

// TODO implement get_type function
std::string_view Document::get_type() const {
    return "DOC";
}
size_t Document::get_raw_size() const {
  // TODO get the document size
    return this->get_size();
}
unsigned Document::get_character_count() const {
  // TODO count non whitespace characters
    const std::string &content_ptr = *this->get_content().get();

    return static_cast<unsigned >(std::count_if(content_ptr.begin(),
                                                content_ptr.end(),
                                                [](unsigned char i)
                                                {return !std::isspace(i);}));
}

// TODO implement content update function
// move new_content to object of derive class: document
void Document::update(FileContent &&new_content) {
    this->content = std::move(new_content);
}