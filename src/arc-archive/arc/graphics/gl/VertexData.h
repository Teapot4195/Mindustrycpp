//
// Created by teapot on 11/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_VERTEXDATA_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_VERTEXDATA_H

#include <memory>
#include <vector>

#include <arc/util/Disposable.h>

// Forward dec lto avoid circular reference
class IndexData;
class Shader;

class VertexData : public Disposable {
public:
    ~VertexData() override = default;

    virtual void render(std::shared_ptr<IndexData> indices, int primitiveType, int offset, int count);

    virtual void size() = 0;

    virtual int max() = 0;

    virtual void set(std::vector<float> vertices, int offset, int count) = 0;

    virtual void update(int targetOffset, std::vector<float> vertices, int sourceOffset, int count) = 0;

    virtual std::vector<float> buffer() = 0;

    virtual void bind(std::shared_ptr<Shader> shader) = 0;

    virtual void unbind(std::shared_ptr<Shader> shader) = 0;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_VERTEXDATA_H
