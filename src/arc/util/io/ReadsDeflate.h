//
// Created by teapot on 02/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_IO_READSDEFLATE_H
#define MINDUSTRY_SRC_ARC_UTIL_IO_READSDEFLATE_H

#include <fstream>
#include <memory>
#include <optional>
#include <vector>
#include <cstdint>

#include <boost/beast/zlib.hpp>

#include "Reads.h"

class ReadsDeflate : public Reads {
private:
    constexpr static const std::size_t buf_size = 1024;
    static std::shared_ptr<ReadsDeflate> instance;
    boost::beast::zlib::inflate_stream stream = boost::beast::zlib::inflate_stream();
    boost::beast::zlib::z_params params = boost::beast::zlib::z_params();
    std::uint8_t inbuf[buf_size]{}, obuf[buf_size * 2]{};
    std::size_t avail_out{0}, obuf_head{0}, avail_in{0};

    std::uint8_t get();

    void fillinbuf();

    void flush();

public:
    std::optional<std::ifstream> input;

    std::vector<std::uint8_t> read(std::size_t size);

    std::uint8_t read();

    ReadsDeflate();

    explicit ReadsDeflate(std::ifstream input);

    explicit ReadsDeflate(const std::shared_ptr<Reads>& read);

    ~ReadsDeflate();

    static std::shared_ptr<ReadsDeflate> get(std::ifstream input);

    int checkEOF() override;

    long long l() override;

    int i() override;

    short s() override;

    unsigned short us() override;

    char b() override;

    std::vector<std::uint8_t> b(int length) override;

    std::vector<std::uint8_t> b(std::vector<std::uint8_t>& array) override;

    unsigned char ub() override;

    bool boolean() override;

    float f() override;

    double d() override;

    std::string str() override;

    void skip(int amount) override;
};

#endif //MINDUSTRY_SRC_ARC_UTIL_IO_READSDEFLATE_H
