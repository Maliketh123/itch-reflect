#include "reflect_decode.hpp"
#include <cstdint>
#include <iostream>
#include <string>

struct Sample {
    char tag;
    std::uint16_t a;
    std::uint32_t b;
    std::uint64_t c;
    char end;
};

struct Pair {
    std::uint32_t x;
    std::uint32_t y;
};

static int failures = 0;

// Compare two numbers; print a line only when they differ.
void check_num(const std::string& name, std::uint64_t got, std::uint64_t want) {
    if (got != want) {
        std::cout << "FAIL " << name << ": got " << got << ", want " << want << '\n';
        ++failures;
    }
}

// Compare two pieces of text; print a line only when they differ.
void check_text(const std::string& name, const std::string& got, const std::string& want) {
    if (got != want) {
        std::cout << "FAIL " << name << ": got '" << got << "', want '" << want << "'\n";
        ++failures;
    }
}

int main() {
    // Sample 1: two 4-byte fields.
    const std::uint8_t pair_message[8] = {
        0x00, 0x00, 0x00, 0x07,  // x = 7
        0x00, 0x00, 0x00, 0x09   // y = 9
    };
    Pair r = decode<Pair>(pair_message);
    check_num("pair.x", r.x, 7);
    check_num("pair.y", r.y, 9);

    // Sample 2: fields of 1, 2, 4, 8 and 1 bytes.
    const std::uint8_t sample_message[16] = {
        0x51,                                            // tag = 'Q'
        0x01, 0x02,                                      // a = 258
        0x00, 0x00, 0x01, 0xF4,                          // b = 500
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,  // c = 42
        0x5A                                             // end = 'Z'
    };
    Sample t = decode<Sample>(sample_message);
    check_text("sample.tag", std::string(1, t.tag), "Q");
    check_num("sample.a", t.a, 258);
    check_num("sample.b", t.b, 500);
    check_num("sample.c", t.c, 42);
    check_text("sample.end", std::string(1, t.end), "Z");

    if (failures == 0) std::cout << "all tests passed\n";
    return failures == 0 ? 0 : 1;
}
