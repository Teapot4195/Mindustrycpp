//
// Created by teapot on 25/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_POOLING_POOL_H
#define MINDUSTRY_SRC_ARC_UTIL_POOLING_POOL_H

#include <limits>
#include <memory>
#include <cstdint>

#include <arc/struct.h>

struct Poolable {
public:
    virtual ~Poolable() = default;

    virtual void reset() = 0;
};

template <typename T>
requires std::derived_from<T, Poolable>
class Pool {
public:
    const std::uint32_t max{};

private:
    Seq<std::shared_ptr<T>> freeObjects;

public:
    int peak{};

    virtual ~Pool() = default;

    Pool();

    explicit Pool(std::uint32_t initialCapacity);

    Pool(std::uint32_t initialCapacity, std::uint32_t m);

    std::shared_ptr<T> obtain();

    void free(std::shared_ptr<T> object);

    void reset(std::shared_ptr<T> object);

    void freeAll(Seq<std::shared_ptr<T>> objects);

    void clear();

    [[nodiscard]] std::size_t getFree() const;

protected:
    virtual std::shared_ptr<T> newObject() = 0;
};

template<typename T>
requires std::derived_from<T, Poolable>
std::size_t Pool<T>::getFree() const {
    return freeObjects.size();
}

template<typename T>
requires std::derived_from<T, Poolable>
void Pool<T>::clear() {
    freeObjects.clear();
}

template<typename T>
requires std::derived_from<T, Poolable>
void Pool<T>::freeAll(Seq<std::shared_ptr<T>> objects) {
    for (std::size_t i = 0; i < objects.size(); i++) {
        std::shared_ptr<T> object = objects[i];
        if (!object)
            continue;
        if (freeObjects.size() < max)
            freeObjects.push_back(object);
        reset(object);
    }
    peak = std::max(peak, freeObjects.size());
}

template<typename T>
requires std::derived_from<T, Poolable>
void Pool<T>::reset(std::shared_ptr<T> object) {
    object.reset();
}

template<typename T>
requires std::derived_from<T, Poolable>
void Pool<T>::free(std::shared_ptr<T> object) {
    if (!object)
        throw std::invalid_argument("object cannot be null.");
    if (freeObjects.size() < max) {
        freeObjects.push_back(object);
        peak = std::max(peak, freeObjects.size());
    }
    reset(object);
}

template<typename T>
requires std::derived_from<T, Poolable>
std::shared_ptr<T> Pool<T>::obtain() {
    if (freeObjects.empty())
        return newObject();
    auto ret = freeObjects.back();
    freeObjects.pop_back();
    return ret;
}

template<typename T>
requires std::derived_from<T, Poolable>
Pool<T>::Pool(std::uint32_t initialCapacity, std::uint32_t m) : max(m) {
    freeObjects = Seq<std::shared_ptr<T>>(initialCapacity);
}

template<typename T>
requires std::derived_from<T, Poolable>
Pool<T>::Pool(std::uint32_t initialCapacity) : Pool(initialCapacity, std::numeric_limits<std::uint32_t>::max()) {}

template<typename T>
requires std::derived_from<T, Poolable>
Pool<T>::Pool() : Pool(16, std::numeric_limits<std::uint32_t>::max()){}


#endif //MINDUSTRY_SRC_ARC_UTIL_POOLING_POOL_H
