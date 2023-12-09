//
// Created by teapot on 25/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_STRUCT_H
#define MINDUSTRY_SRC_ARC_STRUCT_H

#include <vector>

struct Runnable {
    virtual ~Runnable() = default;

    virtual void run() = 0;
};

template <typename T>
using Seq = std::vector<T>;
using IntSeq = Seq<int>;
using LongSeq = Seq<long long>;
using ULongSeq = Seq<unsigned long long>;

#endif //MINDUSTRY_SRC_ARC_STRUCT_H
