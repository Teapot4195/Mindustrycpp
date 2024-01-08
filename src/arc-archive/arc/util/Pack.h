//
// Created by teapot on 03/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_PACK_H
#define MINDUSTRY_SRC_ARC_UTIL_PACK_H

#include <vector>
#include <cstdint>

// Note the masks everywhere, this is because C++ is this weird thing where we can't even guarantee the new shift bits
// won't be trashed
namespace Pack {
    int u(std::uint8_t b) {
        return b & 0xFF;
    }

    int u(short b) {
        return b & 0xFFFF;
    }

    long long u(int b) {
        return b & 0xFFFFFFFFl;
    }

    std::uint8_t byteValue(bool b) {
        return b ? 1 : 0;
    }

    int shortInt(short left, short right) {
        return (int)(left << 16 & 0xFFFF0000l) | (int)(right & 0xFFFFl);
    }

    long long longInt(int x, int y) {
        return (long long)(((long long) x) << 32 & 0xFFFFFFFF00000000ll) | (y & 0xFFFFFFFFll);
    }

    std::uint8_t byteByte(std::uint8_t left, std::uint8_t right) {
        return (std::uint8_t)((left << 4 & 0xf0) | (right & 0xF));
    }

    std::uint8_t leftByte(std::uint8_t value) {
        return (std::uint8_t)((value >> 4) & 0x0F);
    }

    std::uint8_t rightByte(std::uint8_t value) {
        return (std::uint8_t)((value & 0x0F));
    }

    short leftShort(int field) {
        return (short)(field >> 16 & 0xFFFF);
    }

    short rightShort(int field) {
        return (short)(field & 0xFFFF);
    }

    int leftInt(long long field) {
        return (int)(field >> 32 & 0xFFFFFFFFl);
    }

    int rightInt(long long field) {
        return (int)(field & 0xFFFFFFFFl);
    }
    
    std::uint8_t leftByte(short field) {
        return (std::uint8_t)(field >> 8 & 0xFF);
    }
    
    std::uint8_t rightByte(short field) {
        return (std::uint8_t)(field & 0xFF);
    }
    
    short shortByte(std::uint8_t left, std::uint8_t right) {
        return (short)((left << 8 & 0xFF00) | (right & 0xFF));
    }

    std::vector<std::uint8_t> bytes(int i, std::vector<std::uint8_t> result) {
        result[0] = (i >> 24 & 0xFF);
        result[1] = (i >> 16 & 0xFF);
        result[2] = (i >> 8 & 0xFF);
        result[3] = (i /*>> 0*/ & 0xFF);

        return result;
    }

    std::vector<std::uint8_t> bytes(int i) {
        std::vector<std::uint8_t> result;

        result[0] = (i >> 24 & 0xFF);
        result[1] = (i >> 16 & 0xFF);
        result[2] = (i >> 8 & 0xFF);
        result[3] = (i /*>> 0*/ & 0xFF);

        return result;
    }

    std::vector<short> shorts(long long i, std::vector<short> result) {
        result[0] = (short)(i >> 48 & 0xFFFF);
        result[1] = (short)(i >> 32 & 0xFFFF);
        result[2] = (short)(i >> 16 & 0xFFFF);
        result[3] = (short)(i /*>> 0*/ & 0xFFFF);

        return result;
    }

    long long longShort(const std::vector<short> &s) {
        return (long long)((long long)(0xFFFF & s[0]) << 48 & 0xFFFF000000000000ll) |
               (long long)((long long)(0xFFFF & s[1]) << 32 & 0x0000FFFF00000000ll) |
               (long long)((long long)(0xFFFF & s[2]) << 16 & 0x00000000FFFF0000ll) |
               (long long)((long long)(0xFFFF & s[3])       & 0x000000000000FFFFll);
    }

    int intBytes(std::uint8_t b1, std::uint8_t b2, std::uint8_t b3, std::uint8_t b4) {
        return (int)((int)(0xFF & b1) << 24 & 0xFF000000) |
               (int)((int)(0xFF & b2) << 16 & 0x00FF0000) |
               (int)((int)(0xFF & b3) <<  8 & 0x0000FF00) |
               (int)((int)(0xFF & b4)       & 0x000000FF);
    }

    int intBytes(const std::vector<std::uint8_t> &array) {
        return (int)((int)(0xFF & array[0]) << 24 & 0xFF000000) |
               (int)((int)(0xFF & array[1]) << 16 & 0x00FF0000) |
               (int)((int)(0xFF & array[2]) <<  8 & 0x0000FF00) |
               (int)((int)(0xFF & array[3])       & 0x000000FF);
    }
}

#endif //MINDUSTRY_SRC_ARC_UTIL_PACK_H
