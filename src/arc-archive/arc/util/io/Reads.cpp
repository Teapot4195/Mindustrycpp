//
// Created by teapot on 21/11/23.
//

#include "Reads.h"

std::shared_ptr<Reads> Reads::instance = std::make_shared<Reads>();

Reads::Reads() = default;

Reads::Reads(std::ifstream input) : input(std::make_optional<std::ifstream>(std::move(input))) {}

std::shared_ptr<Reads> Reads::get(std::ifstream input) {
    instance->input = std::move(input);
    return instance;
}

int Reads::checkEOF() {
    try {
        return input->get();
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

long long Reads::l() {
    try {
        long long ret;
        char a[8];
        if (input->readsome(a, 8) != 8)
            throw std::runtime_error("Reached EOF before finished reading long value");
        for (char &e : a) {
            ret <<= 8;
            ret &= (long long)0xFFFFFFFFFFFFFF00ll;
            ret |= e;
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

int Reads::i() {
    try {
        int ret;
        char a[4];
        if (input->readsome(a, 4) != 4)
            throw std::runtime_error("Reached EOF before finished reading int value");
        for (char &e : a) {
            ret <<= 8;
            ret &= (int)0xFFFFFF00ll;
            ret |= e;
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

short Reads::s() {
    try {
        short ret;
        char a[2];
        if (input->readsome(a, 2) != 2)
            throw std::runtime_error("Reached EOF before finished reading short value");
        for (char &e : a) {
            ret <<= 8;
            ret &= (short)0xFF00l;
            ret |= e;
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

unsigned short Reads::us() {
    try {
        unsigned short ret;
        char a[2];
        if (input->readsome(a, 2) != 2)
            throw std::runtime_error("Reached EOF before finished reading unsigned short value");
        for (char &e : a) {
            ret <<= 8;
            ret &= 0xFF00l;
            ret |= e;
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

char Reads::b() {
    try {
        char ret;
        if (input->readsome((char*)&ret, 1) != 1)
            throw std::runtime_error("Reached EOF before finished reading byte value");
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

std::vector<std::uint8_t> Reads::b(int length) {
    try {
        std::vector<std::uint8_t> array;
        array.resize(length);
        if (input->readsome((char*)array.data(), length) != length)
            throw std::runtime_error("Reached EOF before finished reading array of " + std::to_string(length) + " bytes");
        return array;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

//TODO: This is a massive performance loss for big arrays!
//Makes a copy of a vector we don't even own, and then returns it, maybe it should return a reference instead????
std::vector<std::uint8_t> Reads::b(std::vector<std::uint8_t> &array) {
    try {
        if (input->readsome((char*) array.data(), array.size()) != array.size())
            throw std::runtime_error("Reached EOF before finished reading array of " + std::to_string(array.size()) + " bytes");
        return array;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

unsigned char Reads::ub() {
    try {
        unsigned char ret;
        if (input->readsome((char*)&ret, 1) != 1)
            throw std::runtime_error("Reached EOF before finished reading byte value");
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

bool Reads::boolean() {
    try {
        char ret;
        if (input->readsome(&ret, 1) != 1)
            throw std::runtime_error("Reached EOF before finished reading bool value");
        return ret != 0;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

float Reads::f() {
    try {
        std::uint32_t ret;
        char a[4];
        if (input->readsome(a, 4) != 4)
            throw std::runtime_error("Reached EOF before finished reading float value");
        for (char &e : a) {
            ret <<= 8;
            ret &= 0xFFFFFF00;
            ret |= e;
        }
        return std::bit_cast<float>(ret);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

double Reads::d() {
    try {
        std::uint64_t ret;
        char a[8];
        if (input->readsome(a, 8) != 8)
            throw std::runtime_error("Reached EOF before finished reading double value");
        for (char &e : a) {
            ret <<= 8;
            ret &= 0xFFFFFFFFFFFFFF00;
            ret |= e;
        }
        return std::bit_cast<double>(ret);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

std::string Reads::str() {
    try {
        //TODO: This does not conform!
        std::string ret;
        std::size_t len = us();
        for (int i = 0; i < len; i++) {
            input->ignore();
            ret += input->get();
        }
        return ret;
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void Reads::skip(int amount) {
    try {
        input->ignore(amount);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

Reads::~Reads() {
    try {
        if (input.has_value())
            input->close();
    }  catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}
