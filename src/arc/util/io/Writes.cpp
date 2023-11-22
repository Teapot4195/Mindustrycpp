//
// Created by teapot on 21/11/23.
//

#include "Writes.h"

//TODO: Make these rethrow exception descriptions way more creative than "Exception was thrown."!

std::shared_ptr<Writes> Writes::instance = std::make_shared<Writes>();

Writes::Writes() = default;

Writes::Writes(std::ofstream output) : output(std::make_optional<std::ofstream>(std::move(output))) {}

std::shared_ptr<Writes> Writes::get(std::ofstream output) {
    instance->output = std::move(output);
    return instance;
}

void Writes::l(long long i) {
    try {
        char a[sizeof(long long)];
        for (char &f : a) {
            f = (char)(i & 0xFFl);
            i >>= 8;
        }
        output->write(a, sizeof(long long));
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Writes::i(int i) {
    try {
        char a[sizeof(int)];
        for (char &f : a) {
            f = (char)(i & 0xFFl);
            i >>= 8;
        }
        output->write(a, sizeof(int));
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Writes::b(int i) {
    try {
        char a = i & 0xFF;
        output->write(&a, 1);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Writes::b(std::vector<std::uint8_t> array, int offset, std::size_t length) {
    try {
        output->write((const char*)array.data(), (std::streamsize)length);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Writes::b(const std::vector<std::uint8_t>& array) {
    b(array, 0, array.size());
}

void Writes::boolean(bool b) {
    this->b(b ? 1 : 0);
}

void Writes::s(int i) {
    try {
        char a[sizeof(short)];
        for (char &f : a) {
            f = (char)(i & 0xFFl);
            i >>= 8;
        }
        output->write(a, sizeof(short));
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Writes::f(float f) {
    try {
        auto i = std::bit_cast<std::uint32_t>(f);
        char a[4];
        for (char &e : a) {
            e = (char)(i & 0xFFl);
            i >>= 8;
        }
        output->write(a, 4);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Writes::d(double d) {
    try {
        auto i = std::bit_cast<std::uint64_t>(d);
        char a[8];
        for (char &e : a) {
            e = (char)(i & 0xFFl);
            i >>= 8;
        }
        output->write(a, 8);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Writes::str(std::string str) {
    try {
        const char* buf = str.c_str();
        output->put((str.size() & 0xFF00) >> 8);
        output->put(str.size() & 0xFF);
        for (int i = 0; i < str.size(); i++) {
            output->put(0x00);
            output->put(buf[i]);
        }
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

Writes::~Writes() {
    if (output.has_value())
        try {
            output->close();
        } catch (const std::exception& e) {
            std::throw_with_nested(std::runtime_error("Exception was thrown."));
        }
}
