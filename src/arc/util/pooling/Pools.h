//
// Created by teapot on 25/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_POOLING_POOLS_H
#define MINDUSTRY_SRC_ARC_UTIL_POOLING_POOLS_H

#include <memory>
#include <mutex>

#include <arc/func.h>
#include "Pool.h"

template <typename T>
class FPool : public Pool<T> {
private:
    Prov<std::shared_ptr<T>> supp;

public:
    using Pool<T>::Pool;

    void setSupp(Prov<std::shared_ptr<T>> supplier) {
        supp = supplier;
    }

protected:
    std::shared_ptr<T> newObject() override {
        return supp();
    }
};

template <typename T>
class Pools {
private:
    static std::shared_ptr<Pool<T>> pool;
    static std::mutex obtain_lock, free_lock;

public:
    Pools() = delete;

    static std::shared_ptr<Pool<T>> get(Prov<std::shared_ptr<T>> supplier, int max);

    static std::shared_ptr<Pool<T>> get(Prov<std::shared_ptr<T>> supplier);

    static void set(std::shared_ptr<Pool<T>> p);

    static std::shared_ptr<T> obtain(Prov<std::shared_ptr<T>> supplier);

    static void free(std::shared_ptr<T> object);

    static void freeAll(std::vector<std::shared_ptr<T>> objects);
};

template <typename T>
static std::shared_ptr<Pool<T>> pool();

template<typename T>
void Pools<T>::freeAll(std::vector<std::shared_ptr<T>> objects) {
    for (int i = 0, n = objects.size(); i < n; i++) {
        std::shared_ptr<T> obj = objects[i];
        if (!obj)
            continue;
        if (!pool)
            continue;
        pool->free(obj);
    }
}

template<typename T>
void Pools<T>::free(std::shared_ptr<T> object) {
    std::lock_guard<std::mutex>lockGuard(free_lock);
    if (!object)
        throw std::invalid_argument("Object cannot be null");
    if (!pool)
        return;
    pool->free(object);
}

template<typename T>
std::shared_ptr<T> Pools<T>::obtain(Prov<std::shared_ptr<T>> supplier) {
    std::lock_guard<std::mutex> lockGuard(obtain_lock);
    return get(supplier)->obtain();
}

template<typename T>
void Pools<T>::set(std::shared_ptr<Pool<T>> p) {
    pool = p;
}

template<typename T>
std::shared_ptr<Pool<T>> Pools<T>::get(Prov<std::shared_ptr<T>> supplier) {
    return get(supplier, 5000);
}

template<typename T>
std::shared_ptr<Pool<T>> Pools<T>::get(Prov<std::shared_ptr<T>> supplier, int max) {
    if (!pool) {
        pool = std::make_shared<FPool<T>>(4, max);
        std::dynamic_pointer_cast<FPool<T>>(pool)->setSupp(supplier);
    }
    return pool;
}

#endif //MINDUSTRY_SRC_ARC_UTIL_POOLING_POOLS_H
