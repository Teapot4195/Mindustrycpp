//
// Created by teapot on 10/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_MESH_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_MESH_H

#include <memory>
#include <vector>

#include <arc/util/Disposable.h>

// Forward decl to avoid circular reference
class VertexAttribute;
class VertexData;
class IndexData;

class Mesh : public Disposable, public std::enable_shared_from_this<Mesh> {
public:
    int vertexSize;
    std::vector<std::shared_ptr<VertexAttribute>> attributes;
    std::shared_ptr<std::shared_ptr<VertexData>> vertices;
    std::shared_ptr<std::shared_ptr<IndexData>> indices;

    bool autoBind = true;

    Mesh(bool isStatic, int maxVertices, int maxIndices, std::vector<std::shared_ptr<VertexAttribute>> attributes);

    Mesh(bool useVertexArray, bool isStatic, int maxVertices, int maxIndices, std::vector<std::shared_ptr<VertexAttribute>> attributes);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_MESH_H
