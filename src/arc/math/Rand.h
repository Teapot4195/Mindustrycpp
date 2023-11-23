//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_RAND_H
#define MINDUSTRY_SRC_ARC_MATH_RAND_H

#include <cstdint>
#include <vector>

class Rand {
private:
    constexpr static const double NORM_DOUBLE = 1.0 / (1L << 53);
    constexpr static const double NORM_FLOAT = 1.0 / (1L << 24);

public:
    std::uint64_t seed0{}, seed1{};

    Rand();

    explicit Rand(std::uint64_t seed);

    Rand(std::uint64_t seed0, std::uint64_t seed1);

    static std::uint64_t murmurHash3(std::uint64_t x);

    std::uint64_t nextLong();

    std::uint32_t next(std::uint8_t bits);

    std::uint32_t nextInt();

    std::uint32_t nextInt(std::uint32_t n);

    std::uint64_t nextLong(std::uint64_t n);

    double nextDouble();

    float nextFloat();

    bool nextBoolean();

    void nextBytes(std::uint8_t *bytes, std::size_t len);

    void nextBytes(std::vector<std::uint8_t>& bytes);

    void setSeed(std::uint64_t seed);

    bool chance(double chance);

    float range(float amount);

    float random(float max);

    std::uint32_t random(std::uint32_t max);

    float random(float min, float max);

    std::uint32_t range(std::uint32_t amount);

    std::uint32_t random(std::uint32_t min, std::uint32_t max);

    void setState(std::uint64_t nseed0, std::uint64_t nseed1);

    [[nodiscard]] std::uint64_t getState(std::uint32_t seed) const;
};


#endif //MINDUSTRY_SRC_ARC_MATH_RAND_H
