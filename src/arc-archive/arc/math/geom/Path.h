//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_PATH_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_PATH_H

#include <memory>

template <typename T>
class Path {
public:
    virtual ~Path() = default;

    virtual std::shared_ptr<T> derivativeAt(std::shared_ptr<T> out, float t) = 0;

    virtual std::shared_ptr<T> valueAt(std::shared_ptr<T> out, float t) = 0;

    virtual float approximate(std::shared_ptr<T> v) = 0;

    virtual float locate(std::shared_ptr<T> v) = 0;

    virtual float approxLength(int samples) = 0;
};


#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_PATH_H
