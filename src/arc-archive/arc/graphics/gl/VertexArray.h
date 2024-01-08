//
// Created by teapot on 11/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_VERTEXARRAY_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_VERTEXARRAY_H

#include <memory>
#include <vector>

#include <arc/graphics/gl/VertexData.h>

// Forward decl to avoid circular reference
class Mesh;

class VertexArray : public VertexData {
private:
    std::shared_ptr<Mesh> mesh;
    std::vector<unsigned char> byteBuffer;
    bool isBound {false};

    std::vector<float> buf();

public:
    VertexArray(int numVertices, std::shared_ptr<Mesh> mesh);

    void render(std::shared_ptr<IndexData> indices, int primitiveType, int offset, int count) override;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_VERTEXARRAY_H
