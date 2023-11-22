//
// Created by teapot on 21/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_IO_READS_H
#define MINDUSTRY_SRC_ARC_UTIL_IO_READS_H

#include <fstream>
#include <memory>
#include <optional>
#include <vector>
#include <cstdint>

class Reads {
private:
    static std::shared_ptr<Reads> instance;

public:
    std::optional<std::ifstream> input;

    Reads();

    Reads(std::ifstream input);

    ~Reads();

    static std::shared_ptr<Reads> get(std::ifstream input);

    int checkEOF();

    long long l();

    int i();

    short s();

    unsigned short us();

    char b();

    std::vector<std::uint8_t> b(int length);

    std::vector<std::uint8_t> b(std::vector<std::uint8_t>& array);

    unsigned char ub();

    bool boolean();

    float f();

    double d();

    std::string str();

    void skip(int amount);
};


#endif //MINDUSTRY_SRC_ARC_UTIL_IO_READS_H
