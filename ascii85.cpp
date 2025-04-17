#include "ascii85.h"
#include <stdexcept>
#include <array>
#include <vector>
#include <cstdint>

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
           
            output.write("z",1);
        } else {
           
            for (int i = 4; i >= 0; --i) {
                encoded[i] = (value % 85) + '!';  
                value /= 85;
            }
            output.write(encoded.data(), count + 1);  
        }
    }
}

void ascii85::decode(std::istream& input, std::ostream& output) {
    std::vector<char> buffer;
    char ch;

    while (input.get(ch)) {
        if (ch == '~') {
            if (input.get(ch) && ch == '>') break;  
            throw std::runtime_error("Invalid ASCII85 termination");
        }

        if (ch == 'z') {
            if (!buffer.empty()) throw std::runtime_error("Invalid 'z' position");
            
            output.write("\0\0\0\0", 4);
        } else if (ch >= '!' && ch <= 'u') {
            buffer.push_back(ch);
            if (buffer.size() == 5) {
                uint32_t value = 0;
             
                for (int i = 0; i < 5; ++i) {
                    value = value * 85 + (buffer[i] - '!');  
                }
                output.put((value >> 24) & 0xFF);
                output.put((value >> 16) & 0xFF);
                output.put((value >> 8) & 0xFF);
                output.put(value & 0xFF);
                buffer.clear();
            }
        } else {
            throw std::runtime_error("Invalid character in ASCII85 encoding");
        }
    }


    if (!buffer.empty()) {
        if (buffer.size() != 5) {
            throw std::runtime_error("Incomplete ASCII85 block");
        }
        while (buffer.size() < 5) buffer.push_back('u');  
        uint32_t value = 0;
        for (int i = 0; i < 5; ++i) {
            value = value * 85 + (buffer[i] - '!'); 
        }

        int byteCount = buffer.size() - 1;
        output.put((value >> 24) & 0xFF);
        if (byteCount > 1) output.put((value >> 16) & 0xFF);
        if (byteCount > 2) output.put((value >> 8) & 0xFF);
        if (byteCount > 3) output.put(value & 0xFF);
    }
}
