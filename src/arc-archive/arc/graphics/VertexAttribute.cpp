//
// Created by teapot on 10/12/23.
//

#include "VertexAttribute.h"

#include <arc/graphics/GL20.h>
#include <arc/graphics/gl/Shader.h>

#include <utility>

const std::shared_ptr<VertexAttribute> position = std::make_shared<VertexAttribute>(2, Shader::positionAttribute);
const std::shared_ptr<VertexAttribute> position3 = std::make_shared<VertexAttribute>(3, Shader::positionAttribute);
const std::shared_ptr<VertexAttribute> texCoords = std::make_shared<VertexAttribute>(2, Shader::texcoordAttribute + "0");
const std::shared_ptr<VertexAttribute> normal = std::make_shared<VertexAttribute>(3, Shader::normalAttribute);
const std::shared_ptr<VertexAttribute> color = std::make_shared<VertexAttribute>(4, GL20::GL_UNSIGNED_BYTE, true, Shader::colorAttribute);
const std::shared_ptr<VertexAttribute> mixColor = std::make_shared<VertexAttribute>(4, GL20::GL_UNSIGNED_BYTE, true, Shader::mixColorAttribute);

VertexAttribute::VertexAttribute(int components, std::string alias) : VertexAttribute(components, GL20::GL_FLOAT, false, std::move(alias)) {}

VertexAttribute::VertexAttribute(int components, int type, bool normalized, std::string alias)
  : components(components), type(type), normalized(normalized), alias(std::move(alias)), size(getRealSize(components, type)) {}

int VertexAttribute::getRealSize(int components, int type) {
    int realSize;
    switch(type){
        case GL20::GL_FLOAT:
        case GL20::GL_FIXED:
            realSize = 4 * components;
            break;
        case GL20::GL_UNSIGNED_BYTE:
        case GL20::GL_BYTE:
            realSize = components;
            break;
        case GL20::GL_UNSIGNED_SHORT:
        case GL20::GL_SHORT:
            realSize = 2 * components;
            break;
        default:
            throw std::invalid_argument("getRealSize got unknown value");
    }
    return realSize;
}
