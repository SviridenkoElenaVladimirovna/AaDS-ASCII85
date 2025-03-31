#include "ascii85.h"
#include <iostream>
#include <string>
#include <sstream>

void print_usage(const std::string& prog_name) {
    std::cerr << "Usage: " << prog_name << " [-e | -d]" << std::endl;
    std::cerr << "  -e : encode (default)" << std::endl;
    std::cerr << "  -d : decode" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string mode = "-e";
    if (argc > 2) {
        print_usage(argv[0]);
        return 1;
    }
    if (argc == 2) {
        mode = argv[1];
    }

    try {
        if (mode == "-e" || mode == "") {
            ascii85::encode(std::cin, std::cout);
            return 0;
        } else if (mode == "-d") {
            std::stringstream error_stream;
            if (!ascii85::decode(std::cin, std::cout, error_stream)) {
                if (!error_stream.str().empty()) {
                    std::cerr << error_stream.str() << std::endl;
                }
                return 1;
            }
            return 0;
        } else {
            print_usage(argv[0]);
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
