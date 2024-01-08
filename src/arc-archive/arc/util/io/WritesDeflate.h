//
// Created by teapot on 01/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_IO_WRITESDEFLATE_H
#define MINDUSTRY_SRC_ARC_UTIL_IO_WRITESDEFLATE_H

#include <memory>
#include <optional>
#include <fstream>
#include <vector>
#include <cstdint>

#include <boost/beast/zlib.hpp>

#include "Writes.h"

class WritesDeflate : public Writes {
private:
    constexpr static const std::size_t buf_size = 1024;
    static std::shared_ptr<WritesDeflate> instance;
    boost::beast::zlib::deflate_stream stream = boost::beast::zlib::deflate_stream();
    boost::beast::zlib::z_params params = boost::beast::zlib::z_params();
    std::uint8_t inbufa[buf_size]{}, inbufb[buf_size]{}, obuf[buf_size]{};
    std::size_t avail_in{0};
    bool usebufa{true};

    void flush();

public:
    std::optional<std::ofstream> output;

    void write(const std::uint8_t* array, std::size_t size);

    void write(std::uint8_t byte);

    WritesDeflate();

    explicit WritesDeflate(std::ofstream output);

    explicit WritesDeflate(const std::shared_ptr<Writes>& write);

    ~WritesDeflate() override;

    static std::shared_ptr<WritesDeflate> get(std::ofstream output);

    void l(long long i) override;

    void i(int i) override;

    void b(int i) override;

    void b(std::vector<std::uint8_t> array, int offset, std::size_t length) override;

    void b(const std::vector<std::uint8_t>& array) override;

    void boolean(bool b) override;

    void s(int i) override;

    void f(float f) override;

    void d(double d) override;

    void str(std::string str) override;
};


#endif //MINDUSTRY_SRC_ARC_UTIL_IO_WRITESDEFLATE_H
