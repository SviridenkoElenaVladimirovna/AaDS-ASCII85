#ifndef ASCII85_H
#define ASCII85_H

#include <iostream>

class ascii85 {
public:
    static void encode(std::istream& input, std::ostream& output);
    static void decode(std::istream& input, std::ostream& output);
};

#endif
