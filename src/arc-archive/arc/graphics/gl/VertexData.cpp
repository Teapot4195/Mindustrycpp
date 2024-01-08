//
// Created by teapot on 11/12/23.
//

#include "VertexData.h"

#include <arc/graphics/Gl.h>
#include <arc/graphics/GL20.h>
#include <arc/graphics/gl/IndexData.h>
#include <arc/util/Exception.h>

void VertexData::render(std::shared_ptr<IndexData> indices, int primitiveType, int offset, int count) {
    if (indices->size() > 0) {
        if (count + offset > indices->max())
            throw ArcRuntimeException("Mesh attempting to access memory outside of the index buffer (count: "
                                      + std::to_string(count) + ", offset: " + std::to_string(offset)
                                      + ", max: " + std::to_string(indices->max()) + ")");

        Gl::drawElements(primitiveType, count, GL20::GL_UNSIGNED_SHORT, offset * 2);
    } else
        Gl::drawArrays(primitiveType, offset, count);
}
