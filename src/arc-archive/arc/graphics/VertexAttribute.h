//
// Created by teapot on 10/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_VERTEXATTRIBUTE_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_VERTEXATTRIBUTE_H

#include <memory>

class VertexAttribute final {
public:
    static const std::shared_ptr<VertexAttribute>
            position, position3, texCoords, normal, color, mixColor;

    const int components;
    const bool normalized;
    const int type;
    const std::string alias;
    const int size;

    VertexAttribute(int components, std::string alias);

    VertexAttribute(int components, int type, bool normalized, std::string alias);

    static int getRealSize(int components, int type);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_VERTEXATTRIBUTE_H
