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

    virtual int checkEOF();

    virtual long long l();

    virtual int i();

    virtual short s();

    virtual unsigned short us();

    virtual char b();

    virtual std::vector<std::uint8_t> b(int length);

    virtual std::vector<std::uint8_t> b(std::vector<std::uint8_t>& array);

    virtual unsigned char ub();

    virtual bool boolean();

    virtual float f();

    virtual double d();

    virtual std::string str();

    virtual void skip(int amount);
};


#endif //MINDUSTRY_SRC_ARC_UTIL_IO_READS_H
