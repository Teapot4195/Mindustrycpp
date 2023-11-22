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

    ~Writes();

    static std::shared_ptr<Writes> get(std::ofstream output);

    void l(long long i);

    void i(int i);

    void b(int i);

    void b(std::vector<std::uint8_t> array, int offset, std::size_t length);

    void b(const std::vector<std::uint8_t>& array);

    void boolean(bool b);

    void s(int i);

    void f(float f);

    void d(double d);

    void str(std::string str);
};


#endif //MINDUSTRY_SRC_ARC_UTIL_IO_WRITES_H
