//
// Created by teapot on 11/12/23.
//

#include "VertexArray.h"

#include <arc/graphics/Mesh.h>
#include <arc/graphics/gl/IndexData.h>

std::vector<float> VertexArray::buf() {
    return {byteBuffer.data(), byteBuffer.data() + byteBuffer.size()};
}

VertexArray::VertexArray(int numVertices, std::shared_ptr<Mesh> mesh) : mesh(mesh) {
    byteBuffer = std::vector<unsigned char>(mesh->vertexSize * numVertices);
}

void VertexArray::render(std::shared_ptr<IndexData> indices, int primitiveType, int offset, int count) {
    if(indices->size() > 0) {
        std::vector<short> b = indices->buffer();
    }
}
