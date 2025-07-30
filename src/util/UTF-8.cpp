#include "UTF-8.hpp"
#include "Logger.hpp"
#include <cstdint>

void UTF8::decode(std::wstring &dest, const std::string &in) {
    size_t length = in.size();
    dest.clear();
    dest.reserve(length); //maximum size (if ASCII characters used alone)

    size_t in_index = 0;
    bool error = false;
    while (in_index < length) {
        uint32_t ch = static_cast<uint32_t>(in[in_index]);
        if ((ch & 0b11100000) == 0b11000000) {
            if (in_index + 1 >= length) {
                error = true;
                break;
            }
            uint32_t ch1 = static_cast<uint32_t>(in[in_index + 1]);
            ch = ((ch & 0b00011111) << 6) | (ch1 & 0b00111111);
            in_index += 2;
        } else if ((ch & 0b11110000) == 0b11100000) {
            if (in_index + 2 >= length) {
                error = true;
                break;
            }

            uint32_t ch1 = static_cast<uint32_t>(in[in_index + 1]);
            uint32_t ch2 = static_cast<uint32_t>(in[in_index + 2]);

            ch = ((ch & 0b00001111) << 12) | ((ch1 & 0b00111111) << 6) | (ch2 & 0b00111111);
            in_index += 3;
        } else if ((ch & 0b11111000) == 0b11110000) {
            if (in_index + 3 >= length) {
                error = true;
                break;
            }

            uint32_t ch1 = static_cast<uint32_t>(in[in_index + 1]);
            uint32_t ch2 = static_cast<uint32_t>(in[in_index + 2]);
            uint32_t ch3 = static_cast<uint32_t>(in[in_index + 3]);

            ch = ((ch & 0b00000111) << 18) | ((ch1 & 0b00111111) << 12) | ((ch2 & 0b00111111) << 6) | (ch3 & 0b00111111);
            in_index += 4;
        } else {
            in_index += 1;
        }

        dest += ch;
    }

    if (error)
        Log::err("Error while decoding UTF-8 string: {}", in);
}
