#include "audio.h"

Audio::Audio(FileContent &&content, unsigned duration)
    : File{std::move(content)}, duration{duration} {}

// TODO implement get_type function
std::string_view Audio::get_type() const {
    return "AUD";
}

size_t Audio::get_raw_size() const {
    return static_cast<size_t>(2*48000*2* this->duration);
    // TODO calculate raw size
}

unsigned Audio::get_duration() { return this->duration; }

// TODO implement content update function
void Audio::update(FileContent &&new_content, unsigned int new_duration) {
    this->content = std::move(new_content);
    this->duration = new_duration;
}