//
// Created by teapot on 25/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_STRUCT_H
#define MINDUSTRY_SRC_ARC_STRUCT_H

#include <array>
#include <vector>
#include <unordered_map>

struct Runnable {
    virtual ~Runnable() = default;

    virtual void run() = 0;
};

template <typename T>
using Seq = std::vector<T>;
using IntSeq = Seq<int>;
using LongSeq = Seq<long long>;
using ULongSeq = Seq<unsigned long long>;

template <typename T, std::size_t size>
using ObjectSet = std::array<T, size>;

template <typename K, typename V>
using ObjectMap = std::unordered_map<K, V>;

#endif //MINDUSTRY_SRC_ARC_STRUCT_H
