//
// Created by teapot on 10/12/23.
//

#include "Mesh.h"

#include <arc/Core.h>
#include <arc/graphics/VertexAttribute.h>

#include <utility>

Mesh::Mesh(bool isStatic, int maxVertices, int maxIndices, std::vector<std::shared_ptr<VertexAttribute>> attributes)
  : Mesh(false, isStatic, maxVertices, maxIndices, std::move(attributes)) {}

Mesh::Mesh(bool useVertexArray, bool isStatic, int maxVertices, int maxIndices,
           std::vector<std::shared_ptr<VertexAttribute>> attributes) {
    int count = 0;
    for (auto& attribute : attributes){
        count += attribute->size;
    }

    vertexSize = count;
    attributes = attributes;

    if (useVertexArray && Core::gl30.has_value()) {
        vertices = std::make_shared<VertexArray>(maxVertices, shared_from_this());
        indices = std::make_shared<IndexArray>(maxIndices);
    } else if(!Core::gl30.has_value()) {
        vertices = std::make_shared<VertexBufferObjectWithVAO>(isStatic, maxVertices, shared_from_this());
        indices = std::make_shared<IndexBufferObject>(isStatic, maxIndices);
    } else {
        vertices = std::make_shared<VertexBufferObject>(isStatic, maxVertices, shared_from_this());
        indices = std::make_shared<IndexBufferObject>(isStatic, maxIndices);
    }
}
