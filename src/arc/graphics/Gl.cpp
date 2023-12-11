//
// Created by teapot on 28/11/23.
//

//TODO: replace "gl.value()->" with "gl.value()->"

#include "Gl.h"

#include <Core.h>
#include <arc/graphics/GL20.h>

#define optimize true

std::vector<int> Gl::ibuf = std::vector<int>(1);
int Gl::lastActiveTexture = -1;
int Gl::lastUsedProgram = 0;
bool Gl::wasDepthMask = true;
int Gl::lastBlendSrc = -1;
int Gl::lastBlendDst = -1;
int Gl::lastBlendSrcAlpha = -1;
int Gl::lastBlendDstAlpha = -1;

sblock(Gl, {
    Gl::reset();
})

void Gl::reset() {
    lastActiveTexture = -1;
    for (int & lastBoundTexture : lastBoundTextures)
        lastBoundTexture = -1;
    lastUsedProgram = 0;
    enabled.clear();
    wasDepthMask = true;
}

void Gl::activeTexture(int tex) {
    if (optimize && lastActiveTexture == tex)
        return;

    Core::gl.value()->glActiveTexture(tex);
    lastActiveTexture = tex;
}

void Gl::bindTexture(int target, int texture) {
    if (optimize && target == (int)Gl::G::texture2d) {
        int index = lastActiveTexture - texture;

        if (index >= 0 && index < lastBoundTexturesL) {
            if (lastBoundTextures[index] == texture)
                return;
            lastBoundTextures[index] = texture;
        }
    }

    Core::gl.value()->glBindTexture(target, texture);
}

void Gl::blendFunc(int sfactor, int dfactor) {
    if (optimize && lastBlendSrc == sfactor && lastBlendDst == dfactor && lastBlendSrcAlpha == sfactor && lastBlendDstAlpha == dfactor)
        return;

    Core::gl.value()->glBlendFunc(lastBlendSrc = lastBlendSrcAlpha = sfactor, lastBlendDst = lastBlendDstAlpha = dfactor);
}

void Gl::clear(int mask) {
    Core::gl.value()->glClear(mask);
}

void Gl::clearColor(float red, float green, float blue, float alpha) {
    Core::gl.value()->glClearColor(red, green, blue, alpha);
}

void Gl::clearDepthf(float depth) {
    Core::gl.value()->glClearDepthf(depth);
}

void Gl::clearStencil(int s) {
    Core::gl.value()->glClearStencil(s);
}

void Gl::colorMask(bool red, bool green, bool blue, bool alpha) {
    Core::gl.value()->glColorMask(red, green, blue, alpha);
}

void
Gl::compressedTexImage2D(int target, int level, int internalformat, int width, int height, int border, int imageSize,
                         Gl::Buf &data) {
    Core::gl.value()->glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

void
Gl::compressedTexSubImage2D(int &target, int &level, int &xoffset, int &yoffset, int &width, int &height, int &format,
                            int &imageSize, Gl::Buf &data) {
    Core::gl.value()->glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void
Gl::copyTexImage2D(int &target, int &level, int &internalformat, int &x, int &y, int &width, int &height, int &border) {
    Core::gl.value()->glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void
Gl::copyTexSubImage2D(int &target, int &level, int &xoffset, int &yoffset, int &x, int &y, int &width, int &height) {
    Core::gl.value()->glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void Gl::cullFace(int &mode) {
    Core::gl.value()->glCullFace(mode);
}

void Gl::deleteTexture(int &texture) {
    for (int & lastBoundTexture : lastBoundTextures) {
        if (lastBoundTexture == texture) {
            lastBoundTexture = -1;
        }
    }

    Core::gl.value()->glDeleteTexture(texture);
}

void Gl::depthFunc(int &func) {
    Core::gl.value()->glDepthFunc(func);
}

void Gl::depthMask(bool &flag) {
    if (optimize && flag == wasDepthMask)
        return;
    wasDepthMask = flag;

    Core::gl.value()->glDepthMask(flag);
}

void Gl::depthRangef(float zNear, float zFar) {
    Core::gl.value()->glDepthRangef(zNear, zFar);
}

void Gl::disable(int &cap) {
    if (optimize && !enabled[cap])
        return;
    Core::gl.value()->glDisable(cap);
    enabled[cap] = false;
}

void Gl::drawArrays(int &mode, int &first, int &count) {
    Core::gl.value()->glDrawArrays(mode, first, count);
}

void Gl::drawElements(int &mode, int &count, int &type, Gl::Buf &indices) {
    Core::gl.value()->glDrawElements(mode, count, type, indices);
}

void Gl::enable(int &cap) {
    if (optimize && enabled[cap])
        return;
    Core::gl.value()->glEnable(cap);
    enabled[cap] = true;
}

void Gl::finish() {
    Core::gl.value()->glFinish();
}

void Gl::flush() {
    Core::gl.value()->glFlush();
}

void Gl::frontFace(int &mode) {
    Core::gl.value()->glFrontFace(mode);
}

int Gl::genTexture() {
    return Core::gl.value()->glGenTexture();
}

int Gl::getError() {
    return Core::gl.value()->glGetError();
}

void Gl::getIntegerv(int &pname, std::vector<int> &params) {
    Core::gl.value()->glGetInterv(pname, params);
}

int Gl::getInt(int &name) {
    Gl::getIntegerv(name, ibuf);
    return ibuf[0];
}

std::string Gl::getString(int &name) {
    return Core::gl.value()->glGetString(name);
}

void Gl::hint(int &target, int &mode) {
    Core::gl.value()->glHint(target, mode);
}

void Gl::pixelStorei(int &pname, int &param) {
    Core::gl.value()->glPixelStorei(pname, param);
}

void Gl::polygonOffset(float factor, float units) {
    Core::gl.value()->glPolygonOffset(factor, units);
}

void Gl::readPixels(int &x, int &y, int &width, int &height, int &format, int &type, Gl::Buf &pixels) {
    Core::gl.value()->glReadPixels(x, y, width, height, format, type, pixels);
}

void Gl::scissor(int &x, int &y, int &width, int &height) {
    Core::gl.value()->glScissor(x, y, width, height);
}

void Gl::stencilFunc(int &func, int &ref, int &mask) {
    Core::gl.value()->glStencilFunc(func, ref, mask);
}

void Gl::stencilMask(int &mask) {
    Core::gl.value()->glStencilMask(mask);
}

void Gl::stencilOp(int &fail, int &zfail, int &zpass) {
    Core::gl.value()->glStencilOp(fail, zfail, zpass);
}

void Gl::texImage2D(int &target, int &level, int &internalformat, int &width, int &height, int &border, int &format,
                    int &type, Gl::Buf &pixels) {
    Core::gl.value()->glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void Gl::texParameterf(int &target, int &pname, float &param) {
    Core::gl.value()->glTexParameterf(target, pname, param);
}

void
Gl::texSubImage2D(int &target, int &level, int &xoffset, int &yoffset, int &width, int &height, int &format, int &type,
                  Gl::Buf &pixels) {
    Core::gl.value()->glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void Gl::viewport(int &x, int &y, int &width, int &height) {
    Core::gl.value()->glViewport(x, y, width, height);
}

void Gl::attachShader(int &program, int &shader) {
    Core::gl.value()->glAttachShader(program, shader);
}

void Gl::bindAttribLocation(int &program, int &index, std::string &name) {
    Core::gl.value()->glBindAttribLocation(program, index, name);
}

void Gl::bindBuffer(int &target, int &buffer) {
    Core::gl.value()->glBindBuffer(target, buffer);
}

void Gl::bindFramebuffer(int &target, int &framebuffer) {
    Core::gl.value()->glBindFramebuffer(target, framebuffer);
}

void Gl::bindRenderbuffer(int &target, int &renderbuffer) {
    Core::gl.value()->glBindRenderbuffer(target, renderbuffer);
}

void Gl::blendColor(float &red, float &green, float &blue, float &alpha) {
    Core::gl.value()->glBlendColor(red, green, blue, alpha);
}

void Gl::blendEquation(int &mode) {
    Core::gl.value()->glBlendEquation(mode);
}

void Gl::blendEquationSeparate(int &modeRGB, int &modeAlpha) {
    Core::gl.value()->glBlendEquationSeparate(modeRGB, modeAlpha);
}

void Gl::blendFuncSeparate(int &srcRGB, int &dstRGB, int &srcAlpha, int &dstAlpha) {
    if (optimize && srcRGB == lastBlendSrc && dstRGB == lastBlendDst && srcAlpha == lastBlendSrcAlpha && dstAlpha == lastBlendDstAlpha)
        return;

    Core::gl.value()->glBlendFuncSeparate(lastBlendSrc = srcRGB,
                                  lastBlendDst = dstRGB,
                                  lastBlendSrcAlpha = srcAlpha,
                                  lastBlendDstAlpha = dstAlpha);
}

void Gl::bufferData(int &target, int &size, Gl::Buf &data, int &usage) {
    Core::gl.value()->glBufferData(target, size, data, usage);
}

void Gl::bufferSubData(int &target, int &offset, int &size, Gl::Buf &data) {
    Core::gl.value()->glBufferSubData(target, offset, size, data);
}

int Gl::checkFramebufferStatus(int &target) {
    return Core::gl.value()->glCheckFramebufferStatus(target);
}

void Gl::compileShader(int &shader) {
    Core::gl.value()->glCompileShader(shader);
}

int Gl::createProgram() {
    return Core::gl.value()->glCreateProgram();
}

int Gl::createShader(int &type) {
    return Core::gl.value()->glCreateShader(type);
}

void Gl::deleteBuffer(int &buffer) {
    Core::gl.value()->glDeleteBuffer(buffer);
}

void Gl::deleteFramebuffer(int &framebuffer) {
    Core::gl.value()->glDeleteFramebuffer(framebuffer);
}

void Gl::deleteProgram(int &program) {
    Core::gl.value()->glDeleteProgram(program);
}

void Gl::deleteRenderbuffer(int &renderbuffer) {
    Core::gl.value()->glDeleteRenderbuffer(renderbuffer);
}

void Gl::deleteShader(int &shader) {
    Core::gl.value()->glDeleteShader(shader);
}

void Gl::detachShader(int &program, int &shader) {
    Core::gl.value()->glDetachShader(program, shader);
}

void Gl::disableVertexAttribArray(int &index) {
    Core::gl.value()->glDisableVertexAttribArray(index);
}

void Gl::drawElements(int &mode, int &count, int &type, int &indices) {
    Core::gl.value()->glDrawElements(mode, count, type, indices);
}

void Gl::enableVertexAttribArray(int &index) {
    Core::gl.value()->glEnableVertexAttribArray(index);
}

void Gl::framebufferRenderbuffer(int &target, int &attachment, int &renderbuffertarget, int &renderbuffer) {
    Core::gl.value()->glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void Gl::framebufferTexture2D(int &target, int &attachment, int &textarget, int &texture, int &level) {
    Core::gl.value()->glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

int Gl::genBuffer() {
    return Core::gl.value()->glGenBuffer();
}

void Gl::generateMipmap(int &target) {
    Core::gl.value()->glGenerateMipmap(target);
}

int Gl::genFramebuffer() {
    return Core::gl.value()->glGenFramebuffer();
}

int Gl::genRenderbuffer() {
    return Core::gl.value()->glGenRenderbuffer();
}

std::string Gl::getActiveAttrib(int &program, int &index, std::vector<int> &size, std::vector<int> &type) {
    return Core::gl.value()->glGetActiveAttrib(program, index, size, type);
}

std::string Gl::getActiveUniform(int &program, int &index, std::vector<int> &size, std::vector<int> &type) {
    return Core::gl.value()->glGetActiveUniform(program, index, size, type);
}

int Gl::getAttribLocation(int program, std::string name) {
    return Core::gl.value()->glGetAttribLocation(program, name);
}

void Gl::getBooleanv(int &pname, Gl::Buf &params) {
    Core::gl.value()->glGetBooleanv(pname, params);
}

void Gl::getBufferParameteriv(int &target, int &pname, std::vector<int> &params) {
    Core::gl.value()->glGetBufferParameteriv(target, pname, params);
}

void Gl::getFloatv(int &pname, std::vector<float> &params) {
    Core::gl.value()->glGetFloatv(pname, params);
}

void Gl::getFramebufferAttachmentParameteriv(int &target, int &attachment, int &pname, std::vector<int> &params) {
    Core::gl.value()->glGetFramebufferAttachmentparameteriv(target, attachment, pname, params);
}

void Gl::getProgramiv(int &program, int pname, std::vector<int> &params) {
    Core::gl.value()->glGetProgramiv(program, pname, params);
}

std::string Gl::getProgramInfoLog(int &program) {
    return Core::gl.value()->glGetProgramInfoLog(program);
}

void Gl::getRenderbufferParameteriv(int &target, int &pname, std::vector<int> &params) {
    Core::gl.value()->glGetRenderbufferParameteriv(target, pname, params);
}

void Gl::getShaderiv(int &shader, int pname, std::vector<int> &params) {
    Core::gl.value()->glGetShaderiv(shader, pname, params);
}

std::string Gl::getShaderInfoLog(int &shader) {
    return Core::gl.value()->glGetShaderInfoLog(shader);
}

void Gl::getShaderPrecisionFormat(int &shadertype, int &precisiontype, std::vector<int> &range,
                                  std::vector<int> &precision) {
    Core::gl.value()->glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

void Gl::getTexParameterfv(int &target, int &pname, std::vector<float> &params) {
    Core::gl.value()->glGetTexParameterfv(target, pname, params);
}

void Gl::getTexParameteriv(int &target, int &pname, std::vector<int> &params) {
    Core::gl.value()->glGetTexParameteriv(target, pname, params);
}

void Gl::getUniformfv(int &program, int &location, std::vector<float> &params) {
    Core::gl.value()->glGetUniformfv(program, location, params);
}

void Gl::getUniformiv(int &program, int &location, std::vector<int> &params) {
    Core::gl.value()->glGetUniformiv(program, location, params);
}

int Gl::getUniformLocation(int &program, std::string &name) {
    return Core::gl.value()->glGetUniformLocation(program, name);
}

void Gl::getVertexAttribfv(int &index, int &pname, std::vector<float> &params) {
    Core::gl.value()->glGetVertexAttribfv(index, pname, params);
}

void Gl::getVertexAttribiv(int &index, int &pname, std::vector<int> &params) {
    Core::gl.value()->glGetVertexAttribiv(index, pname, params);
}

bool Gl::isBuffer(int &buffer) {
    return Core::gl.value()->glIsBuffer(buffer);
}

bool Gl::isEnabled(int &cap) {
    return Core::gl.value()->glIsEnabled(cap);
}

bool Gl::isFramebuffer(int &framebuffer) {
    return Core::gl.value()->glIsFramebuffer(framebuffer);
}

bool Gl::isProgram(int &program) {
    return Core::gl.value()->glIsProgram(program);
}

bool Gl::isRenderbuffer(int &renderbuffer) {
    return Core::gl.value()->glIsRenderbuffer(renderbuffer);
}

bool Gl::isShader(int &shader) {
    return Core::gl.value()->glIsShader(shader);
}

bool Gl::isTexture(int &texture) {
    return Core::gl.value()->glIsTexture(texture);
}

void Gl::linkProgram(int &program) {
    Core::gl.value()->glLinkProgram(program);
}

void Gl::releaseShaderCompiler() {
    Core::gl.value()->glReleaseShaderCompiler();
}

void Gl::renderbufferStorage(int &target, int &internalformat, int &width, int &height) {
    Core::gl.value()->glRenderbufferStorage(target, internalformat, width, height);
}

void Gl::sampleCoverage(float &value, bool &invert) {
    Core::gl.value()->glSampleCoverage(value, invert);
}

void Gl::shaderSource(int &shader, std::string &string) {
    Core::gl.value()->glShaderSource(shader, string);
}

void Gl::stencilFuncSeparate(int &face, int &func, int &ref, int &mask) {
    Core::gl.value()->glStencilFuncSeparate(face, func, ref, mask);
}

void Gl::stencilMaskSeparate(int &face, int &mask) {
    Core::gl.value()->glStencilMaskSeparate(face, mask);
}

void Gl::stencilOpSeparate(int &face, int &fail, int &zfail, int &zpass) {
    Core::gl.value()->glStencilOpSeparate(face, fail, zfail, zpass);
}

void Gl::texParameterfv(int &target, int &pname, std::vector<float> &params) {
    Core::gl.value()->glTexParameterfv(target, pname, params);
}

void Gl::texParameteri(int &target, int &pname, int &param) {
    Core::gl.value()->glTexParameteri(target, pname, param);
}

void Gl::texParameteriv(int &target, int &pname, std::vector<int> &params) {
    Core::gl.value()->glTexParameteriv(target, pname, params);
}

void Gl::uniform1f(int &location, float &x) {
    Core::gl.value()->glUniform1f(location, x);
}

void Gl::uniform1fv(int &location, int &count, std::vector<float> &v) {
    Core::gl.value()->glUniform1fv(location, count, v);
}

void Gl::uniform1fv(int &location, int count, std::vector<float> &v, int offset) {
    Core::gl.value()->glUniform1fv(location, count, v, offset);
}

void Gl::uniform1i(int &location, int &x) {
    Core::gl.value()->glUniform1i(location, x);
}

void Gl::uniform1iv(int &location, int &count, std::vector<int> &v) {
    Core::gl.value()->glUniform1iv(location, count, v);
}

void Gl::uniform1iv(int &location, int &count, std::vector<int> &v, int &offset) {
    Core::gl.value()->glUniform1iv(location, count, v, offset);
}

void Gl::uniform2f(int &location, float &x, float &y) {
    Core::gl.value()->glUniform2f(location, x, y);
}

void Gl::uniform2fv(int &location, int &count, std::vector<float> &v) {
    Core::gl.value()->glUniform2fv(location, count, v);
}

void Gl::uniform2fv(int &location, int count, std::vector<float> &v, int offset) {
    Core::gl.value()->glUniform2fv(location, count, v, offset);
}

void Gl::uniform2i(int &location, int &x, int &y) {
    Core::gl.value()->glUniform2i(location, x, y);
}

void Gl::uniform2iv(int &location, int &count, std::vector<int> &v) {
    Core::gl.value()->glUniform2iv(location, count, v);
}

void Gl::uniform2iv(int &location, int &count, std::vector<int> &v, int &offset) {
    Core::gl.value()->glUniform2iv(location, count, v, offset);
}

void Gl::uniform3f(int &location, float &x, float &y, float &z) {
    Core::gl.value()->glUniform3f(location, x, y, z);
}

void Gl::uniform3fv(int &location, int &count, std::vector<float> &v) {
    Core::gl.value()->glUniform3fv(location, count, v);
}

void Gl::uniform3fv(int &location, int count, std::vector<float> &v, int offset) {
    Core::gl.value()->glUniform3fv(location, count, v, offset);
}

void Gl::uniform3i(int &location, int &x, int &y, int &z) {
    Core::gl.value()->glUniform3i(location, x, y, z);
}

void Gl::uniform3iv(int &location, int &count, std::vector<int> &v) {
    Core::gl.value()->glUniform3iv(location, count, v);
}

void Gl::uniform3iv(int &location, int &count, std::vector<int> &v, int &offset) {
    Core::gl.value()->glUniform3iv(location, count, v, offset);
}

void Gl::uniform4f(int &location, float &x, float &y, float &z, float &w) {
    Core::gl.value()->glUniform4f(location, x, y, z, w);
}

void Gl::uniform4fv(int &location, int &count, std::vector<float> &v) {
    Core::gl.value()->glUniform4fv(location, count, v);
}

void Gl::uniform4fv(int &location, int count, std::vector<float> &v, int offset) {
    Core::gl.value()->glUniform4fv(location, count, v, offset);
}

void Gl::uniform4i(int &location, int &x, int &y, int &z, int &w) {
    Core::gl.value()->glUniform4i(location, x, y, z, w);
}

void Gl::uniform4iv(int &location, int &count, std::vector<int> &v) {
    Core::gl.value()->glUniform4iv(location, count, v);
}

void Gl::uniform4iv(int &location, int &count, std::vector<int> &v, int &offset) {
    Core::gl.value()->glUniform4iv(location, count, v, offset);
}

void Gl::uniformMatrix2fv(int &location, int &count, bool &transpose, std::vector<float> &value) {
    Core::gl.value()->glUniformMatrix2fv(location, count, transpose, value);
}

void Gl::uniformMatrix2fv(int &location, int &count, bool &transpose, std::vector<float> &value, int &offset) {
    Core::gl.value()->glUniformMatrix2fv(location, count, transpose, value, offset);
}

void Gl::uniformMatrix3fv(int &location, int count, bool &transpose, std::vector<float> value) {
    Core::gl.value()->glUniformMatrix3fv(location, count, transpose, value);
}

void Gl::uniformMatrix3fv(int &location, int count, bool &transpose, std::vector<float> value, int offset) {
    Core::gl.value()->glUniformMatrix3fv(location, count, transpose, value, offset);
}

void Gl::uniformMatrix4fv(int location, int count, bool transpose, std::vector<float> &value) {
    Core::gl.value()->glUniformMatrix4fv(location, count, transpose, value);
}

void Gl::uniformMatrix4fv(int location, int count, bool transpose, std::vector<float> &value, int offset) {
    Core::gl.value()->glUniformMatrix4fv(location, count, transpose, value, offset);
}

void Gl::useProgram(int &program) {
    if (optimize && lastUsedProgram == program)
        return;

    Core::gl.value()->glUseProgram(program);
    lastUsedProgram = program;
}

void Gl::validateProgram(int &program) {
    Core::gl.value()->glValidateProgram(program);
}

void Gl::vertexAttrib1f(int &indx, float &x) {
    Core::gl.value()->glVertexAttrib1f(indx, x);
}

void Gl::vertexAttrib1fv(int &indx, std::vector<float> &values) {
    Core::gl.value()->glVertexAttrib1fv(indx, values);
}

void Gl::vertexAttrib2f(int &indx, float &x, float &y) {
    Core::gl.value()->glVertexAttrib2f(indx, x, y);
}

void Gl::vertexAttrib2fv(int &indx, std::vector<float> &values) {
    Core::gl.value()->glVertexAttrib2fv(indx, values);
}

void Gl::vertexAttrib3f(int &indx, float &x, float &y, float &z) {
    Core::gl.value()->glVertexAttrib3f(indx, x, y, z);
}

void Gl::vertexAttrib3fv(int &indx, std::vector<float> &values) {
    Core::gl.value()->glVertexAttrib3fv(indx, values);
}

void Gl::vertexAttrib4f(int &indx, float &x, float &y, float &z, float &w) {
    Core::gl.value()->glVertexAttrib4f(indx, x, y, z, w);
}

void Gl::vertexAttrib4fv(int &indx, std::vector<float> &values) {
    Core::gl.value()->glVertexAttrib4fv(indx, values);
}

void Gl::vertexAttribPointer(int &indx, int &size, int &type, bool &normalized, int &stride, Gl::Buf &ptr) {
    Core::gl.value()->glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

void Gl::vertexAttribPointer(int &indx, int &size, int &type, bool &normalized, int &stride, int &ptr) {
    Core::gl.value()->glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}
