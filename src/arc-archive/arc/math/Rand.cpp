//
// Created by teapot on 23/11/23.
//

#include "Rand.h"

#include <random>
#include <limits>

Rand::Rand() {
    std::random_device rd;
    setSeed(std::uniform_int_distribution<std::uint64_t>(0, std::numeric_limits<std::uint64_t>::max())(rd));
}

Rand::Rand(std::uint64_t seed) {
    setSeed(seed);
}

Rand::Rand(std::uint64_t seed0, std::uint64_t seed1) {
    setState(seed0, seed1);
}

std::uint64_t Rand::murmurHash3(std::uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53L;
    x ^= x >> 33;

    return x;
}

std::uint64_t Rand::nextLong() {
    std::uint64_t s1 = seed0;
    const std::uint64_t s0 = seed1;
    seed0 = s0;
    s1 ^= s1 << 23;
    return (seed1 = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26))) + s0;
}

std::uint32_t Rand::next(std::uint8_t bits) {
    return (int)(nextLong() & ((1L << bits) - 1));
}

std::uint32_t Rand::nextInt() {
    //TODO: We are slicing the top 32 bits off the random number, perhaps we xor the high and low?
    return nextLong();
}

std::uint32_t Rand::nextInt(const std::uint32_t n) {
    //TODO: Slicing top 32 bits off the random number, we could xor high and low?
    return nextLong(n);
}

std::uint64_t Rand::nextLong(const std::uint64_t n) {
    // We will never need to check for positive n, it is unsigned
    for (;;) {
        const std::uint64_t bits = nextLong() >> 1;
        const std::uint64_t value = bits % n;
        if (bits - value + (n - 1) >= 0)
            return value;
    }
}

double Rand::nextDouble() {
    return (double)(nextLong() >> 11) * NORM_DOUBLE;
}

float Rand::nextFloat() {
    return (float)((double)(nextLong() >> 40) * NORM_FLOAT);
}

bool Rand::nextBoolean() {
    return (nextLong() & 1) != 0;
}

void Rand::nextBytes(std::uint8_t *bytes, std::size_t len) {
    std::size_t n;
    std::size_t i = len;
    while (i != 0) {
        n = i < 8 ? i : 8;
        for (auto bits = nextLong(); n-- != 0; bits >>= 8)
            bytes[--i] = bits;
    }
}

void Rand::nextBytes(std::vector<std::uint8_t> &bytes) {
    nextBytes(bytes.data(), bytes.size());
}

void Rand::setSeed(const std::uint64_t seed) {
    std::uint64_t nseed0 = murmurHash3(seed == 0 ? std::numeric_limits<std::uint64_t>::min() : seed);
    setState(nseed0, murmurHash3(nseed0));
}

bool Rand::chance(double chance) {
    return nextDouble() < chance;
}

float Rand::range(float amount) {
    return nextFloat() * amount * 2 - amount;
}

float Rand::random(float max) {
    return nextFloat() * max;
}

std::uint32_t Rand::random(std::uint32_t max) {
    return nextInt(max + 1);
}

float Rand::random(float min, float max) {
    return min + (max - min) * nextFloat();
}

std::uint32_t Rand::range(std::uint32_t amount) {
    return nextInt(amount * 2 + 1) - amount;
}

std::uint32_t Rand::random(std::uint32_t min, std::uint32_t max) {
    if (min >= max)
        return min;
    return min + nextInt(max - min + 1);
}

void Rand::setState(const std::uint64_t nseed0, const std::uint64_t nseed1) {
    this->seed0 = nseed0;
    this->seed1 = nseed1;
}

std::uint64_t Rand::getState(std::uint32_t seed) const {
    return seed == 0 ? seed0 : seed1;
}
