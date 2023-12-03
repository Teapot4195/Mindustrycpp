//
// Created by teapot on 02/12/23.
//

#include "ReadsDeflate.h"

#include "util/ArcRuntimeException.h"

std::shared_ptr<ReadsDeflate> ReadsDeflate::instance = std::make_shared<ReadsDeflate>();

ReadsDeflate::ReadsDeflate() = default;

ReadsDeflate::ReadsDeflate(std::ifstream input) : input(std::make_optional<std::ifstream>(std::move(input))) {}

ReadsDeflate::ReadsDeflate(const std::shared_ptr<Reads>& read) : input(std::move(read->input)) {}

std::shared_ptr<ReadsDeflate> ReadsDeflate::get(std::ifstream input) {
    instance->input = std::move(input);
    return instance;
}

int ReadsDeflate::checkEOF() {
    try {
        return input->get();
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

long long ReadsDeflate::l() {
    try {
        long long ret;
        for (std::size_t i = 0; i < 8; i++) {
            char e = (char)read();
            ret <<= 8;
            ret &= (long long)0xFFFFFFFFFFFFFF00ll;
            ret |= e;
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

int ReadsDeflate::i() {
    try {
        int ret;
        for (std::size_t i = 0; i < 4; i++) {
            char e = (char)read();
            ret <<= 8;
            ret &= (int)0xFFFFFF00ll;
            ret |= e;
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

short ReadsDeflate::s() {
    try {
        short ret;
        for (std::size_t i = 0; i < 2; i++) {
            ret <<= 8;
            ret &= (short)0xFF00l;
            ret |= read();
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

unsigned short ReadsDeflate::us() {
    try {
        unsigned short ret;
        for (std::size_t i = 0; i < 2; i++) {
            char e = (char) read();
            ret <<= 8;
            ret &= 0xFF00l;
            ret |= e;
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

char ReadsDeflate::b() {
    try {
        char ret = (char) read();
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

std::vector<std::uint8_t> ReadsDeflate::b(int length) {
    try {
        return read(length);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

//TODO: This is a massive performance loss for big arrays!
//Makes a copy of a vector we don't even own, and then returns it, maybe it should return a reference instead????
std::vector<std::uint8_t> ReadsDeflate::b(std::vector<std::uint8_t> &array) {
    try {
        array = read(array.size());
        return array;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

unsigned char ReadsDeflate::ub() {
    try {
        return read();
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

bool ReadsDeflate::boolean() {
    try {
        return read() != 0;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

float ReadsDeflate::f() {
    try {
        std::uint32_t ret;
        for (std::size_t i = 0; i < 4; i++) {
            ret <<= 8;
            ret &= 0xFFFFFF00;
            ret |= read();
        }
        return std::bit_cast<float>(ret);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

double ReadsDeflate::d() {
    try {
        std::uint64_t ret;
        for (std::size_t i = 0; i < 8; i++) {
            ret <<= 8;
            ret &= 0xFFFFFFFFFFFFFF00;
            ret |= read();
        }
        return std::bit_cast<double>(ret);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

std::string ReadsDeflate::str() {
    try {
        //TODO: This does not conform!
        std::string ret;
        std::size_t len = us();
        for (int i = 0; i < len; i++) {
            read();
            ret += (char)read();
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void ReadsDeflate::skip(int amount) {
    try {
        for (std::size_t i = 0; i < amount; i++)
            read();
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

ReadsDeflate::~ReadsDeflate() {
    try {
        if (input.has_value())
            input->close();
    }  catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void ReadsDeflate::flush() {
    assert(avail_out == 0 /*"flush() in ReadsDeflate was called before all current data was consumed!"*/);

    fillinbuf();

    params.next_in = inbuf;
    params.avail_in = avail_in;
    params.next_out = obuf;
    params.avail_out = avail_out;
    boost::system::error_code ecode{};
    stream.write(params, boost::beast::zlib::Flush::none, ecode);

    // TODO: Process errors!

    obuf_head = 0;
    avail_out = params.avail_out;
}

void ReadsDeflate::fillinbuf() {
    avail_in = input->readsome(reinterpret_cast<char *>(inbuf + params.avail_in), (long long)buf_size - (long long)params.avail_in);
}

std::vector<std::uint8_t> ReadsDeflate::read(std::size_t size) {
    std::vector<std::uint8_t> ret = std::vector<std::uint8_t>(size);

    for (std::size_t i = 0; i < size; i++)
        ret[i] = get();

    return ret;
}

std::uint8_t ReadsDeflate::read() {
    return get();
}

std::uint8_t ReadsDeflate::get() {
    if (avail_out == 0)
        flush();
    avail_out--;
    return obuf[obuf_head++];
}
