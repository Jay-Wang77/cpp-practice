#pragma once

#include <optional>
#include <string_view>

namespace bencode {
// TODO: Implement int parsing for the bencode fromat
// Bencode encoded integers have the format `i<number>e`. Your function should return the number.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the integer number,
// which might be negative.
//
// We assume that the passed string_view contains only one encoding, and the last character
// of a valid encoding is 'e'. Therefore, you do not need to handle cases like 'i4ei42e'.
//
// Example of a valid encoding: "i42e", which should be parsed to the integer 42.
// Example of a valid encoding: "i-42e", which should be parsed to the integer -42.
//
// Edges cases you might need to look out for:
// - String not starting with 'i', or ending with 'e'
// - Handle empty string
// - Handle if a non-digit number is between 'i' and 'e'
consteval std::optional<int> parse_int(std::string_view str) {
    if(str.empty() || str.front() != 'i' || str.back() != 'e'){
        return std::nullopt;
    }

    str.remove_prefix(1);
    str.remove_suffix(1);
    if (str.empty()) {
    return std::nullopt;
    }
    int value = 0;
    bool isNegative = false;
    size_t index = 0;

    if (str[index] == '-') {
        isNegative = true;
        ++index;
        if (str.size() == 1) { // 只有一个负号
            return std::nullopt;
        }
    }
    while (index < str.size()) {
        char c = str[index];
        if (c < '0' || c > '9') {
            return std::nullopt; // 非数字字符
        }
        value = value * 10 + (c - '0');
        ++index;
    }
    return isNegative ? -value : value;
}

// TODO: Implement byte string parsing for the bencode fromat
// Bencode byte strings have the format <length>:<string>, where `length` is a number.
// The colon ':' separates the length and the actual string.
// The function returns a `std::optional<std::string_view>` to indicate success or failure.
// In the case of success, the optional should have the value of the string.
// For failure, the optional should be empty.
//
// Example of a valid byte string: "5:hello", where the length is 5, and the string is "hello".
//
// You may assume that all characters in the byte string are ASCII characters.
//
// We do not check for all edge cases, important edgecases your implementation should handle:
// - The byte string doesn't start with a number
// - The byte string doesn't have a colon
// - It is fine for the length to be shorter than the string, just return the string according to the length
// - It is NOT valid for the string to be shorter than the specified length
// - The string may contain colons
consteval std::optional<std::string_view> parse_byte_string(std::string_view str) {
    if (str.empty()) {
        return std::nullopt;
    }
    int length = 0;
    int index = 0;
    while (index < str.size() && str[index] != ':') {
        if (str[index] < '0' || str[index] > '9') {
            return std::nullopt;
        }
        length = length * 10 + (str[index] - '0');
        index++;
        }

    if (str.size() < static_cast<size_t>(index + length)) {
        return std::nullopt;
    }
    index++;
    if (str.size() < static_cast<size_t>(index + length)) {
        return std::nullopt;
    }

    return std::string_view(str.data() + index, length);
}
} // namespace bencode
