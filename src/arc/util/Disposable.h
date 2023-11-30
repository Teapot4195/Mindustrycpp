//
// Created by teapot on 22/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_DISPOSABLE_H
#define MINDUSTRY_SRC_ARC_UTIL_DISPOSABLE_H

class Disposable {
public:
    virtual ~Disposable() = default;
    virtual void dispose() = 0;
    virtual bool isDisposed() {
        return false;
    }
};

#endif //MINDUSTRY_SRC_ARC_UTIL_DISPOSABLE_H
