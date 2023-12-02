//
// Created by teapot on 21/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_IO_WRITES_H
#define MINDUSTRY_SRC_ARC_UTIL_IO_WRITES_H

#include <memory>
#include <optional>
#include <fstream>
#include <vector>
#include <cstdint>

class Writes {
private:
    static std::shared_ptr<Writes> instance;

public:
    std::optional<std::ofstream> output;

    Writes();

    Writes(std::ofstream output);

    virtual ~Writes();

    static std::shared_ptr<Writes> get(std::ofstream output);

    virtual void l(long long i);

    virtual void i(int i);

    virtual void b(int i);

    virtual void b(std::vector<std::uint8_t> array, int offset, std::size_t length);

    virtual void b(const std::vector<std::uint8_t>& array);

    virtual void boolean(bool b);

    virtual void s(int i);

    virtual void f(float f);

    virtual void d(double d);

    virtual void str(std::string str);
};


#endif //MINDUSTRY_SRC_ARC_UTIL_IO_WRITES_H
