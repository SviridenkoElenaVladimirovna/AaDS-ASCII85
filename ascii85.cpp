#include "ascii85.h"
#include <stdexcept>
#include <array>
#include <vector>
#include <cstdint>
#include <iostream>

void ascii85::encode(std::istream& input, std::ostream& output) {
    std::array<unsigned char, 4> buffer{};
    std::array<char, 5> encoded;

    while (input.read(reinterpret_cast<char*>(buffer.data()), 4) || input.gcount() > 0) {
        size_t count = input.gcount();

        if (count < 4) {
            std::fill(buffer.begin() + count, buffer.end(), 0);
        }

        uint32_t value = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];

        if (value == 0 && count == 4) {
            output.put('z');
        } else {
            for (int i = 4; i >= 0; --i) {
                encoded[i] = (value % 85) + 33;
                value /= 85;
            }
            output.write(encoded.data(), count + 1);
        }
    }
}

bool ascii85::decode(std::istream& input, std::ostream& output, std::ostream& error) {
    std::vector<char> buffer;
    char ch;
    bool hasData = false;

    while (input.get(ch)) {
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
            continue;
        }

        if (ch == 'z') {
            if (!buffer.empty()) {
                error << "Invalid 'z' in middle of block";
                return false;
            }
            output.write("\0\0\0\0", 4);
            hasData = true;
            continue;
        }

        if (ch < '!' || ch > 'u') {
            error << "Invalid character in ASCII85 encoding: " << ch;
            return false;
        }

        buffer.push_back(ch);
        hasData = true;

        if (buffer.size() == 5) {
            uint32_t value = 0;
            for (char c : buffer) {
                value = value * 85 + (c - '!');
            }
            output.put((value >> 24) & 0xFF);
            output.put((value >> 16) & 0xFF);
            output.put((value >> 8) & 0xFF);
            output.put(value & 0xFF);
            buffer.clear();
        }
    }

    if (!hasData) {
        return true;
    }

    if (!buffer.empty()) {
        if (buffer.size() == 1) {
            error << "Incomplete ASCII85 block";
            return false;
        }

        size_t bytesToWrite = buffer.size() - 1;
        while (buffer.size() < 5) {
            buffer.push_back('u');
        }

        uint32_t value = 0;
        for (char c : buffer) {
            value = value * 85 + (c - '!');
        }

        if (bytesToWrite >= 1) output.put((value >> 24) & 0xFF);
        if (bytesToWrite >= 2) output.put((value >> 16) & 0xFF);
        if (bytesToWrite >= 3) output.put((value >> 8) & 0xFF);
        if (bytesToWrite >= 4) output.put(value & 0xFF);
    }

    return true;
}
