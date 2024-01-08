//
// Created by teapot on 10/12/23.
//

#include "Shader.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include <arc/Application.h>
#include <arc/Core.h>
#include <arc/Graphics.h>
#include <arc/files/Fi.h>
#include <arc/graphics/Color.h>
#include <arc/graphics/Gl.h>
#include <arc/graphics/GL20.h>
#include <arc/graphics/gl/GLVersion.h>
#include <arc/math/Mat.h>
#include <arc/math/geom/Vec2.h>
#include <arc/math/geom/Vec3.h>
#include <arc/util/Log.h>

bool Shader::pedantic = false;

Shader::Shader(std::string vertexShader, std::string fragmentShader) {
    if (vertexShader.empty())
        throw std::invalid_argument("vertex shader must not be null");
    if (fragmentShader.empty())
        throw std::invalid_argument("fragment shader must not be null");

    vertexShader = preprocess(vertexShader, false);
    fragmentShader = preprocess(fragmentShader, true);

    if (!prependVertexCode.empty())
        vertexShader = prependVertexCode + vertexShader;
    if (!prependFragmentCode.empty())
        fragmentShader = prependFragmentCode + fragmentShader;

    vertexShaderSource = vertexShader;
    fragmentShaderSource = fragmentShader;

    compileShaders(vertexShader, fragmentShader);
    if (isCompiled())  {
        fetchAttributes();
        fetchUniform();
    } else
        throw std::invalid_argument("Failed to compile shader: " + log);
}

Shader::Shader(const std::shared_ptr<Fi>& vertexShader, const std::shared_ptr<Fi>& fragmentShader)
  : Shader(vertexShader->readString(), fragmentShader->readString()) {
    if (!log.empty())
        Log::warn("Shader " + vertexShader->toString() + " | " + fragmentShader->toString() + ":\n" + log);
}

void Shader::apply() {}

std::string Shader::preprocess(std::string source, bool fragment) {
    if (source.contains("#ifdef GL_ES"))
        throw ArcRuntimeException("Shader contains GL_ES specific code; this should be handled by the preprocessor. Code: \n```\n" + source + "\n```");

    if (source.contains("#version"))
        throw ArcRuntimeException("Shader contains explicit version requirement; this should be handled by the preprocessor. Code: \n```\n" + source + "\n```");

    using namespace std::string_literals;

    if (fragment)
        source =
                "#ifdef GL_ES\n"s +
                "precision " + (source.contains("#define HIGHP") && !source.contains("//#define HIGHP") ? "highp" : "mediump") + " float;\n" +
                "precision mediump int;\n" +
                "#else\n" +
                "#define lowp  \n" +
                "#define mediump \n" +
                "#define highp \n" +
                "#endif\n" + source;
    else
        source =
                "#ifndef GL_ES\n"s +
                "#define lowp  \n" +
                "#define mediump \n" +
                "#define highp \n" +
                "#endif\n" + source;

    if (Core::gl30.has_value()) {
        std::string version =
                source.contains("#version ") ? "" :
                Core::app.value()->isDesktop() ? (Core::graphics.value()->getGLVersion()->atLeast(3, 2) ? "150" : "130") :
                "300 es";

        source = util::replaceAll(source, "varying", fragment ? "in" : "out");
        source = util::replaceAll(source, "attribute", fragment ? "???" : "in");
        source = util::replaceAll(source, "texture2D(", "texture(");
        source = util::replaceAll(source, "textureCube(", "texture(");
        source = util::replaceAll(source, "gl_FragColor", "fragColor");

        return
                "#version " + version + "\n"
                + (fragment ? "out lowp vec4 fragColor;\n" : "")
                + source;
    }
    return source;
}

void Shader::compileShaders(std::string vertexShader, std::string fragmentShader) {
    vertexShaderHandle = loadShader(GL20::GL_VERTEX_SHADER, std::move(vertexShader));
    fragmentShaderHandle = loadShader(GL20::GL_FRAGMENT_SHADER, std::move(fragmentShader));

    if (vertexShaderHandle == -1 || fragmentShaderHandle == -1) {
        isCompiled_ = false;
        return;
    }

    program = linkProgram(createProgram());
    if (program == -1) {
        isCompiled_ = false;
        return;
    }

    isCompiled_ = true;
}

int Shader::loadShader(int type_, std::string source) {
    std::vector<int> intbuf;

    int shader = Gl::createShader(type_);
    if (shader == 0)
        return -1;

    Gl::shaderSource(shader, source);
    Gl::compileShader(shader);
    Gl::getShaderiv(shader, GL20::GL_COMPILE_STATUS, intbuf);

    std::string infoLog = Gl::getShaderInfoLog(shader);
    if (!infoLog.empty()) {
        log += type_ == GL20::GL_VERTEX_SHADER ? "Vertex shader\n" : "Fragment shader:\n";
        log += infoLog;
    }

    int compiled = intbuf[0];
    if (compiled == 0) {
        return -1;
    }

    return shader;
}

int Shader::createProgram() {
    int prog = Gl::createProgram();
    return prog != 0 ? prog : -1;
}

int Shader::linkProgram(int prog) {
    if (prog == -1)
        return -1;

    Gl::attachShader(prog, vertexShaderHandle);
    Gl::attachShader(prog, fragmentShaderHandle);
    Gl::linkProgram(prog);

    std::vector<int> intbuf;

    Gl::getProgramiv(prog, GL20::GL_LINK_STATUS, intbuf);
    int linked = intbuf[0];
    if (linked == 0) {
        log = Gl::getProgramInfoLog(prog);
        return -1;
    }

    return prog;
}

std::string Shader::getLog() {
    if(isCompiled_) {
        log = Gl::getProgramInfoLog(program);
        return log;
    } else
        return log;
}

bool Shader::isCompiled() const {
    return isCompiled_;
}

int Shader::fetchAttributeLocation(const std::string& name) const {
    // -2 == not yet cached
    // -1 == cached but not found
    int location;
    if (attributes.contains(name))
        location = attributes[name];
    else {
        location = Gl::getAttribLocation(program, name);
        attributes[name] = location;
    }
    return location;
}

int Shader::fetchUniformLocation(std::string name) {
    return fetchUniformLocation(std::move(name), pedantic);
}

int Shader::fetchUniformLocation(std::string name, bool pedantic_) {
    // -2 == not yet cached
    // -1 == cached but not found
    int location;
    if (uniforms.contains(name))
        location = uniforms[name];
    else {
        location = Gl::getUniformLocation(program, name);
        if (location == -1 && pedantic_)
            throw std::invalid_argument("no uniform with name '" + name + "' in shader");
        uniforms[name] = location;
    }
    return location;
}

void Shader::setUniformi(std::string name, int value) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform1i(location, value);
}

void Shader::setUniformi(int location, int value) {
    Gl::uniform1i(location, value);
}

void Shader::setUniformi(std::string name, int value1, int value2) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform2i(location, value1, value2);
}

void Shader::setUniformi(int location, int value1, int value2) {
    Gl::uniform2i(location, value1, value2);
}

void Shader::setUniformi(std::string name, int value1, int value2, int value3) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform3i(location, value1, value2, value3);
}

void Shader::setUniformi(int location, int value1, int value2, int value3) {
    Gl::uniform3i(location, value1, value2, value3);
}

void Shader::setUniformi(std::string name, int value1, int value2, int value3, int value4) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform4i(location, value1, value2, value3, value4);
}

void Shader::setUniformi(int location, int value1, int value2, int value3, int value4) {
    Gl::uniform4i(location, value1, value2, value3, value4);
}

void Shader::setUniformf(std::string name, float value) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform1f(location, value);
}

void Shader::setUniformf(int location, float value) {
    Gl::uniform1f(location, value);
}

void Shader::setUniformf(std::string name, float value1, float value2) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform2f(location, value1, value2);
}

void Shader::setUniformf(int location, float value1, float value2) {
    Gl::uniform2f(location, value1, value2);
}

void Shader::setUniformf(std::string name, float value1, float value2, float value3) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform3f(location, value1, value2, value3);
}

void Shader::setUniformf(int location, float value1, float value2, float value3) {
    Gl::uniform3f(location, value1, value2, value3);
}

void Shader::setUniformf(std::string name, float value1, float value2, float value3, float value4) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform4f(location, value1, value2, value3, value4);
}

void Shader::setUniformf(int location, float value1, float value2, float value3, float value4) {
    Gl::uniform4f(location, value1, value2, value3, value4);
}

void Shader::setUniform1fv(std::string name, std::vector<float> values, int offset, int length) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform1fv(location, length, values, offset);
}

void Shader::setUniform1fv(int location, std::vector<float> values, int offset, int length) {
    Gl::uniform1fv(location, length, values, offset);
}

void Shader::setUniform2fv(std::string name, std::vector<float> values, int offset, int length) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform2fv(location, length / 2, values, offset);
}

void Shader::setUniform2fv(int location, std::vector<float> values, int offset, int length) {
    Gl::uniform2fv(location, length / 2, values, offset);
}

void Shader::setUniform3fv(std::string name, std::vector<float> values, int offset, int length) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform3fv(location, length / 3, values, offset);
}

void Shader::setUniform3fv(int location, std::vector<float> values, int offset, int length) {
    Gl::uniform3fv(location, length / 3, values, offset);
}

void Shader::setUniform4fv(std::string name, std::vector<float> values, int offset, int length) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniform4fv(location, length / 4, values, offset);
}

void Shader::setUniform4fv(int location, std::vector<float> values, int offset, int length) {
    Gl::uniform4fv(location, length / 4, values, offset);
}

void Shader::setUniformMatrix(std::string name, const std::shared_ptr<Mat>& matrix) {
    setUniformMatrix(std::move(name), matrix, false);
}

void Shader::setUniformMatrix(std::string name, const std::shared_ptr<Mat>& matrix, bool transpose) {
    setUniformMatrix(fetchUniformLocation(std::move(name)), matrix, transpose);
}

void Shader::setUniformMatrix(int location, const std::shared_ptr<Mat>& matrix) {
    setUniformMatrix(location, matrix, false);
}

void Shader::setUniformMatrix(int location, const std::shared_ptr<Mat>& matrix, bool transpose) {
    Gl::uniformMatrix3fv(location, 1, transpose, std::vector<float>(matrix->val, matrix->val + 9), 0);
}

void Shader::setUniformMatrix4(std::string name, std::vector<float> val, bool transpose) {
    Gl::uniformMatrix4fv(fetchUniformLocation(std::move(name)), 1, false, val, 0);
}

void Shader::setUniformMatrix4(std::string name, const std::shared_ptr<Mat> &matrix) {
    Gl::uniformMatrix4fv(fetchUniformLocation(std::move(name)), 1, false, copyTransform(matrix), 0);
}

void Shader::setUniformMatrix4(std::string name, const std::shared_ptr<Mat> &matrix, float near, float far) {
    Gl::uniformMatrix4fv(fetchUniformLocation(std::move(name)), 1, false, copyTransform(matrix, near, far), 0);
}

void Shader::setUniformMatrix3fv(std::string name, std::vector<float> buffer, int count, bool transpose) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniformMatrix3fv(location, count, transpose, std::move(buffer));
}

void Shader::setUniformMatrix4fv(std::string name, std::vector<float> buffer, int count, bool transpose) {
    int location = fetchUniformLocation(std::move(name));
    Gl::uniformMatrix4fv(location, count, transpose, buffer);
}

void Shader::setUniformMatrix4fv(int location, std::vector<float> values, int offset, int length) {
    Gl::uniformMatrix4fv(location, length / 16, false, values, offset);
}

void Shader::setUniformMatrix4fv(std::string name, std::vector<float> values, int offset, int length) {
    setUniformMatrix4fv(fetchUniformLocation(std::move(name)), std::move(values), offset, length);
}

void Shader::setUniformf(std::string name, const std::shared_ptr<Vec2>& values) {
    setUniformf(std::move(name), values->x, values->y);
}

void Shader::setUniformf(int location, const std::shared_ptr<Vec2>& values) {
    setUniformf(location, values->x, values->y);
}

void Shader::setUniformf(std::string name, const std::shared_ptr<Vec3>& values) {
    setUniformf(std::move(name), values->x, values->y, values->z);
}

void Shader::setUniformf(int name, const std::shared_ptr<Vec3>& values) {
    setUniformf(name, values->x, values->y, values->z);
}

void Shader::setUniformf(std::string name, const std::shared_ptr<Color>& values) {
    setUniformf(std::move(name), values->r_, values->g_, values->b_, values->a_);
}

void Shader::setUniformf(int location, const std::shared_ptr<Color>& values) {
    setUniformf(location, values->r_, values->g_, values->b_, values->a_);
}

void Shader::bind() const {
    Gl::useProgram(program);
}

void Shader::dispose() {
    if (disposed)
        return;

    Gl::useProgram(0);
    Gl::deleteShader(vertexShaderHandle);
    Gl::deleteShader(fragmentShaderHandle);
    Gl::deleteProgram(program);
    disposed = true;
}

bool Shader::isDisposed() {
    return disposed;
}

void Shader::disableVertexAttribute(const std::string& name) {
    int location = fetchAttributeLocation(name);
    if (location == -1)
        return;
    Gl::disableVertexAttribArray(location);
}

void Shader::fetchUniform() {
    params.clear();
    Gl::getProgramiv(program, GL20::GL_ACTIVE_UNIFORMS, params);
    int numUniforms = params[0];

    uniformNames = std::vector<std::string>(numUniforms);

    for(int i = 0; i < numUniforms; i++){
        params.clear();
        params[0] = 1;
        type.clear();
        std::string name = Gl::getActiveUniform(program, i, params, type);
        int location = Gl::getUniformLocation(program, name);
        uniforms[name] = location;
        uniformTypes[name] = type[0];
        uniformSizes[name] = params[0];
        uniformNames[i] = name;
    }
}

void Shader::fetchAttributes() {
    params.clear();
    Gl::getProgramiv(program, GL20::GL_ACTIVE_ATTRIBUTES, params);
    int numAttributes = params[0];

    attributeNames = std::vector<std::string>(numAttributes);

    for(int i = 0; i < numAttributes; i++){
        params.clear();
        params[0] = 1;
        type.clear();
        std::string name = Gl::getActiveAttrib(program, i, params, type);
        int location = Gl::getAttribLocation(program, name);
        attributes[name] = location;
        attributeTypes[name] = type[0];
        attributeSizes[name] = params[0];
        attributeNames[i] = name;
    }
}

bool Shader::hasAttribute(const std::string& name) {
    return attributes.contains(name);
}

int Shader::getAttributeType(const std::string& name) {
    if (attributeTypes.contains(name))
        return attributeTypes[name];
    return 0;
}

int Shader::getAttributeLocation(const std::string& name) {
    if (attributes.contains(name))
        return attributes[name];
    return -1;
}

int Shader::getAttributeSize(const std::string& name) {
    if (attributeSizes.contains(name))
        return attributeSizes[name];
    return 0;
}

bool Shader::hasUniform(const std::string& name) {
    return uniforms.contains(name);
}

int Shader::getUniformType(const std::string& name) {
    if (uniformTypes.contains(name))
        return uniformTypes[name];
    return 0;
}

int Shader::getUniformLocation(const std::string& name) {
    if (uniforms.contains(name))
        return uniforms[name];
    return -1;
}

int Shader::getUniformSize(const std::string& name) {
    if (uniformSizes.contains(name))
        return uniforms[name];
    return 0;
}

std::vector<std::string> Shader::getAttributes() {
    return attributeNames;
}

std::vector<std::string> Shader::getUniforms() {
    return uniformNames;
}

std::string Shader::getVertexShaderSource() {
    return vertexShaderSource;
}

std::string Shader::getFragmentShaderSource() {
    return fragmentShaderSource;
}

std::vector<float> Shader::copyTransform(const std::shared_ptr<Mat>& matrix) {
    val[4] = matrix->val[Mat::M01];
    val[1] = matrix->val[Mat::M10];

    val[0] = matrix->val[Mat::M00];
    val[5] = matrix->val[Mat::M11];
    val[10] = matrix->val[Mat::M22];
    val[12] = matrix->val[Mat::M02];
    val[13] = matrix->val[Mat::M12];
    val[15] = 1;
    return {val, val + 16};
}

std::vector<float> Shader::copyTransform(const std::shared_ptr<Mat>& matrix, float near, float far) {
    val[4] = matrix->val[Mat::M01];
    val[1] = matrix->val[Mat::M10];

    val[0] = matrix->val[Mat::M00];
    val[5] = matrix->val[Mat::M11];
    val[10] = matrix->val[Mat::M22];
    val[12] = matrix->val[Mat::M02];
    val[13] = matrix->val[Mat::M12];
    val[15] = 1;

    float z_orth = -2 / (far - near);
    float tz = -(far + near) / (far - near);

    val[10] = z_orth;
    val[14] = tz;

    return {val, val + 16};
}
