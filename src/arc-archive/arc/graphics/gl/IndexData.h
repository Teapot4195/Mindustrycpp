//
// Created by teapot on 11/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_INDEXDATA_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_INDEXDATA_H

#include <vector>

#include <arc/util/Disposable.h>

class IndexData : public Disposable {
public:
    ~IndexData() override = default;

    virtual int size() = 0;

    virtual int max() = 0;

    virtual void set(std::vector<short> indices, int offset, int count) = 0;

    virtual void set(std::vector<short> indices) = 0;

    virtual void update(int targetOffset, std::vector<short> indices, int offset, int count) = 0;

    virtual std::vector<short> buffer() = 0;

    virtual void bind() = 0;

    virtual void unbind() = 0;

    void dispose() override = 0;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_INDEXDATA_H
