#include <gtest/gtest.h>
#include <sstream>
#include "ascii85.h" 

TEST(Ascii85Test, EncodeEmptyInput) {
    std::ostringstream output;
    std::istringstream input("");
    ascii85::encode(input, output);
    EXPECT_EQ(output.str(), "");
}

TEST(Ascii85Test, EncodeSingleBlock) {
    std::ostringstream output;
    std::istringstream input("hell");
    ascii85::encode(input, output);
    EXPECT_EQ(output.str(), "BOu!r");
}


TEST(Ascii85Test, EncodePartialBlock) {
    std::ostringstream output;
    std::istringstream input("he");
    ascii85::encode(input, output);
    EXPECT_EQ(output.str(), "BOq");
}

TEST(Ascii85Test, DecodeEmptyInput) {
    std::ostringstream output;
    std::istringstream input("");
    ascii85::decode(input, output);
    EXPECT_EQ(output.str(), "");
}

TEST(Ascii85Test, DecodeSingleBlock) {
    std::ostringstream output;
    std::istringstream input("BOu!r");
    ascii85::decode(input, output);
    EXPECT_EQ(output.str(), "hell");
}

TEST(Ascii85Test, DecodeZeroBlock) {
    std::ostringstream output;
    std::istringstream input("z");
    ascii85::decode(input, output);
    EXPECT_EQ(output.str(), std::string(4, '\0'));  
}

TEST(Ascii85Test, EncodeZeroBlock) {
    std::vector<unsigned char> input = {0, 0, 0, 0};
    std::ostringstream output;
    std::istringstream input_stream(std::string(input.begin(), input.end()));
    ascii85::encode(input_stream, output);
    EXPECT_EQ(output.str(), "z");
}


TEST(Ascii85Test, DecodeInvalidCharacter) {
    std::ostringstream output;
    std::istringstream input("BOu!r ");  
    EXPECT_THROW(ascii85::decode(input, output), std::runtime_error);
}

TEST(Ascii85Test, DecodeIncompleteBlock) {
    std::ostringstream output;
    std::istringstream input("B");  
    EXPECT_THROW(ascii85::decode(input, output), std::runtime_error);
}
TEST(Ascii85Test, DecodeInvalidCharacterThrowsException) {
    std::istringstream input("BOu!r ");  
    std::ostringstream output;  
    std::streambuf* cinBackup = std::cin.rdbuf(input.rdbuf());  
    std::streambuf* coutBackup = std::cout.rdbuf(output.rdbuf());  
    EXPECT_THROW(ascii85::decode(std::cin, std::cout), std::runtime_error);
    std::cin.rdbuf(cinBackup);
    std::cout.rdbuf(coutBackup);
}

TEST(Ascii85Test, EncodeMultipleBytes) {
    std::istringstream input("Hello");  
    std::ostringstream output;
    ascii85::encode(input, output);
    EXPECT_EQ(output.str(), "87cURDZ");  
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
