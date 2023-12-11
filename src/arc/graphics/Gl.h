//
// Created by teapot on 28/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_H

#include <vector>
#include <string>
#include <cstdint>

#include <Utils.h>

class Gl {

    using ntemplate = std::uint8_t;
    using Buf = std::vector<ntemplate>;

public:
    enum class G {
            esVersion20 = 1,
            depthBufferBit = 0x00000100,
            stencilBufferBit = 0x00000400,
            colorBufferBit = 0x00004000,
            falseV = 0,
            trueV = 1,
            points = 0x0000,
            lines = 0x0001,
            lineLoop = 0x0002,
            lineStrip = 0x0003,
            triangles = 0x0004,
            triangleStrip = 0x0005,
            triangleFan = 0x0006,
            zero = 0,
            one = 1,
            srcColor = 0x0300,
            oneMinusSrcColor = 0x0301,
            srcAlpha = 0x0302,
            oneMinusSrcAlpha = 0x0303,
            dstAlpha = 0x0304,
            oneMinusDstAlpha = 0x0305,
            dstColor = 0x0306,
            oneMinusDstColor = 0x0307,
            srcAlphaSaturate = 0x0308,
            funcAdd = 0x8006,
            blendEquation = 0x8009,
            blendEquationRgb = 0x8009,
            blendEquationAlpha = 0x883D,
            funcSubtract = 0x800A,
            funcReverseSubtract = 0x800B,
            min = 0x8007,
            max = 0x8008,
            blendDstRgb = 0x80C8,
            blendSrcRgb = 0x80C9,
            blendDstAlpha = 0x80CA,
            blendSrcAlpha = 0x80CB,
            constantColor = 0x8001,
            oneMinusConstantColor = 0x8002,
            constantAlpha = 0x8003,
            oneMinusConstantAlpha = 0x8004,
            blendColor = 0x8005,
            arrayBuffer = 0x8892,
            elementArrayBuffer = 0x8893,
            arrayBufferBinding = 0x8894,
            elementArrayBufferBinding = 0x8895,
            streamDraw = 0x88E0,
            staticDraw = 0x88E4,
            dynamicDraw = 0x88E8,
            bufferSize = 0x8764,
            bufferUsage = 0x8765,
            currentVertexAttrib = 0x8626,
            front = 0x0404,
            back = 0x0405,
            frontAndBack = 0x0408,
            texture2d = 0x0DE1,
            cullFace = 0x0B44,
            blend = 0x0BE2,
            dither = 0x0BD0,
            stencilTest = 0x0B90,
            depthTest = 0x0B71,
            scissorTest = 0x0C11,
            polygonOffsetFill = 0x8037,
            sampleAlphaToCoverage = 0x809E,
            sampleCoverage = 0x80A0,
            noError = 0,
            invalidEnum = 0x0500,
            invalidValue = 0x0501,
            invalidOperation = 0x0502,
            outOfMemory = 0x0505,
            cw = 0x0900,
            ccw = 0x0901,
            lineWidth = 0x0B21,
            aliasedPointSizeRange = 0x846D,
            aliasedLineWidthRange = 0x846E,
            cullFaceMode = 0x0B45,
            frontFace = 0x0B46,
            depthRange = 0x0B70,
            depthWritemask = 0x0B72,
            depthClearValue = 0x0B73,
            depthFunc = 0x0B74,
            stencilClearValue = 0x0B91,
            stencilFunc = 0x0B92,
            stencilFail = 0x0B94,
            stencilPassDepthFail = 0x0B95,
            stencilPassDepthPass = 0x0B96,
            stencilRef = 0x0B97,
            stencilValueMask = 0x0B93,
            stencilWritemask = 0x0B98,
            stencilBackFunc = 0x8800,
            stencilBackFail = 0x8801,
            stencilBackPassDepthFail = 0x8802,
            stencilBackPassDepthPass = 0x8803,
            stencilBackRef = 0x8CA3,
            stencilBackValueMask = 0x8CA4,
            stencilBackWritemask = 0x8CA5,
            viewport = 0x0BA2,
            scissorBox = 0x0C10,
            colorClearValue = 0x0C22,
            colorWritemask = 0x0C23,
            unpackAlignment = 0x0CF5,
            packAlignment = 0x0D05,
            maxTextureSize = 0x0D33,
            maxTextureUnits = 0x84E2,
            maxViewportDims = 0x0D3A,
            subpixelBits = 0x0D50,
            redBits = 0x0D52,
            greenBits = 0x0D53,
            blueBits = 0x0D54,
            alphaBits = 0x0D55,
            depthBits = 0x0D56,
            stencilBits = 0x0D57,
            polygonOffsetUnits = 0x2A00,
            polygonOffsetFactor = 0x8038,
            textureBinding2d = 0x8069,
            sampleBuffers = 0x80A8,
            samples = 0x80A9,
            sampleCoverageValue = 0x80AA,
            sampleCoverageInvert = 0x80AB,
            numCompressedTextureFormats = 0x86A2,
            compressedTextureFormats = 0x86A3,
            dontCare = 0x1100,
            fastest = 0x1101,
            nicest = 0x1102,
            generateMipmap = 0x8191,
            generateMipmapHint = 0x8192,
            byteV = 0x1400,
            unsignedByte = 0x1401,
            shortV = 0x1402,
            unsignedShort = 0x1403,
            intV = 0x1404,
            unsignedInt = 0x1405,
            floatV = 0x1406,
            fixed = 0x140C,
            depthComponent = 0x1902,
            alpha = 0x1906,
            rgb = 0x1907,
            rgba = 0x1908,
            luminance = 0x1909,
            luminanceAlpha = 0x190A,
            unsignedShort4444 = 0x8033,
            unsignedShort5551 = 0x8034,
            unsignedShort565 = 0x8363,
            fragmentShader = 0x8B30,
            vertexShader = 0x8B31,
            maxVertexAttribs = 0x8869,
            maxVertexUniformVectors = 0x8DFB,
            maxVaryingVectors = 0x8DFC,
            maxCombinedTextureImageUnits = 0x8B4D,
            maxVertexTextureImageUnits = 0x8B4C,
            maxTextureImageUnits = 0x8872,
            maxFragmentUniformVectors = 0x8DFD,
            shaderType = 0x8B4F,
            deleteStatus = 0x8B80,
            linkStatus = 0x8B82,
            validateStatus = 0x8B83,
            attachedShaders = 0x8B85,
            activeUniforms = 0x8B86,
            activeUniformMaxLength = 0x8B87,
            activeAttributes = 0x8B89,
            activeAttributeMaxLength = 0x8B8A,
            shadingLanguageVersion = 0x8B8C,
            currentProgram = 0x8B8D,
            never = 0x0200,
            less = 0x0201,
            equal = 0x0202,
            lequal = 0x0203,
            greater = 0x0204,
            notequal = 0x0205,
            gequal = 0x0206,
            always = 0x0207,
            keep = 0x1E00,
            replace = 0x1E01,
            incr = 0x1E02,
            decr = 0x1E03,
            invert = 0x150A,
            incrWrap = 0x8507,
            decrWrap = 0x8508,
            vendor = 0x1F00,
            renderer = 0x1F01,
            version = 0x1F02,
            extensions = 0x1F03,
            nearest = 0x2600,
            linear = 0x2601,
            nearestMipmapNearest = 0x2700,
            linearMipmapNearest = 0x2701,
            nearestMipmapLinear = 0x2702,
            linearMipmapLinear = 0x2703,
            textureMagFilter = 0x2800,
            textureMinFilter = 0x2801,
            textureWrapS = 0x2802,
            textureWrapT = 0x2803,
            texture = 0x1702,
            textureCubeMap = 0x8513,
            textureBindingCubeMap = 0x8514,
            textureCubeMapPositiveX = 0x8515,
            textureCubeMapNegativeX = 0x8516,
            textureCubeMapPositiveY = 0x8517,
            textureCubeMapNegativeY = 0x8518,
            textureCubeMapPositiveZ = 0x8519,
            textureCubeMapNegativeZ = 0x851A,
            maxCubeMapTextureSize = 0x851C,
            texture0 = 0x84C0,
            texture1 = 0x84C1,
            texture2 = 0x84C2,
            texture3 = 0x84C3,
            texture4 = 0x84C4,
            texture5 = 0x84C5,
            texture6 = 0x84C6,
            texture7 = 0x84C7,
            texture8 = 0x84C8,
            texture9 = 0x84C9,
            texture10 = 0x84CA,
            texture11 = 0x84CB,
            texture12 = 0x84CC,
            texture13 = 0x84CD,
            texture14 = 0x84CE,
            texture15 = 0x84CF,
            texture16 = 0x84D0,
            texture17 = 0x84D1,
            texture18 = 0x84D2,
            texture19 = 0x84D3,
            texture20 = 0x84D4,
            texture21 = 0x84D5,
            texture22 = 0x84D6,
            texture23 = 0x84D7,
            texture24 = 0x84D8,
            texture25 = 0x84D9,
            texture26 = 0x84DA,
            texture27 = 0x84DB,
            texture28 = 0x84DC,
            texture29 = 0x84DD,
            texture30 = 0x84DE,
            texture31 = 0x84DF,
            activeTexture = 0x84E0,
            repeat = 0x2901,
            clampToEdge = 0x812F,
            mirroredRepeat = 0x8370,
            floatVec2 = 0x8B50,
            floatVec3 = 0x8B51,
            floatVec4 = 0x8B52,
            intVec2 = 0x8B53,
            intVec3 = 0x8B54,
            intVec4 = 0x8B55,
            boolean = 0x8B56,
            boolVec2 = 0x8B57,
            boolVec3 = 0x8B58,
            boolVec4 = 0x8B59,
            floatMat2 = 0x8B5A,
            floatMat3 = 0x8B5B,
            floatMat4 = 0x8B5C,
            sampler2d = 0x8B5E,
            samplerCube = 0x8B60,
            vertexAttribArrayEnabled = 0x8622,
            vertexAttribArraySize = 0x8623,
            vertexAttribArrayStride = 0x8624,
            vertexAttribArrayType = 0x8625,
            vertexAttribArrayNormalized = 0x886A,
            vertexAttribArrayPointer = 0x8645,
            vertexAttribArrayBufferBinding = 0x889F,
            implementationColorReadType = 0x8B9A,
            implementationColorReadFormat = 0x8B9B,
            compileStatus = 0x8B81,
            infoLogLength = 0x8B84,
            shaderSourceLength = 0x8B88,
            shaderCompiler = 0x8DFA,
            shaderBinaryFormats = 0x8DF8,
            numShaderBinaryFormats = 0x8DF9,
            lowFloat = 0x8DF0,
            mediumFloat = 0x8DF1,
            highFloat = 0x8DF2,
            lowInt = 0x8DF3,
            mediumInt = 0x8DF4,
            highInt = 0x8DF5,
            framebuffer = 0x8D40,
            renderbuffer = 0x8D41,
            rgba4 = 0x8056,
            rgb5A1 = 0x8057,
            rgb565 = 0x8D62,
            depthComponent16 = 0x81A5,
            stencilIndex = 0x1901,
            stencilIndex8 = 0x8D48,
            renderbufferWidth = 0x8D42,
            renderbufferHeight = 0x8D43,
            renderbufferInternalFormat = 0x8D44,
            renderbufferRedSize = 0x8D50,
            renderbufferGreenSize = 0x8D51,
            renderbufferBlueSize = 0x8D52,
            renderbufferAlphaSize = 0x8D53,
            renderbufferDepthSize = 0x8D54,
            renderbufferStencilSize = 0x8D55,
            framebufferAttachmentObjectType = 0x8CD0,
            framebufferAttachmentObjectName = 0x8CD1,
            framebufferAttachmentTextureLevel = 0x8CD2,
            framebufferAttachmentTextureCubeMapFace = 0x8CD3,
            colorAttachment0 = 0x8CE0,
            depthAttachment = 0x8D00,
            stencilAttachment = 0x8D20,
            none = 0,
            framebufferComplete = 0x8CD5,
            framebufferIncompleteAttachment = 0x8CD6,
            framebufferIncompleteMissingAttachment = 0x8CD7,
            framebufferIncompleteDimensions = 0x8CD9,
            framebufferUnsupported = 0x8CDD,
            framebufferBinding = 0x8CA6,
            renderbufferBinding = 0x8CA7,
            maxRenderbufferSize = 0x84E8,
            invalidFramebufferOperation = 0x0506,
            vertexProgramPointSize = 0x8642
    };

private:
    static std::vector<int> ibuf;
    static int lastActiveTexture;
    constexpr static const int lastBoundTexturesL = 32;
    static int lastBoundTextures[lastBoundTexturesL];
    static int lastUsedProgram;
    static std::vector<bool> enabled;
    static bool wasDepthMask;
    static int lastBlendSrc, lastBlendDst, lastBlendSrcAlpha, lastBlendDstAlpha;

    sblockfdcl()

public:
    static void reset();

    static void activeTexture(int tex);

    static void bindTexture(int target, int texture);

    static void blendFunc(int sfactor, int dfactor);

    static void clear(int mask);

    static void clearColor(float red, float gree, float blue, float alpha);

    static void clearDepthf(float depth);

    static void clearStencil(int s);

    static void colorMask(bool red, bool green, bool blue, bool alpha);

    static void compressedTexImage2D(int target, int level, int internalformat, int width, int height, int border, int imageSize, Buf& data);

    static void compressedTexSubImage2D(int &target, int &level ,int &xoffset, int &yoffset, int &width, int &height, int &format, int &imageSize, Buf& data);

    static void copyTexImage2D(int &target, int &level, int &internalformat, int &x, int &y, int &width, int &height, int &border);

    static void copyTexSubImage2D(int &target, int &level, int &xoffset, int &yoffset, int &x, int &y, int &width, int &height);

    static void cullFace(int &mode);

    static void deleteTexture(int &texture);

    static void depthFunc(int &func);

    static void depthMask(bool &flag);

    static void depthRangef(float zNear, float zFar);

    static void disable(int &cap);

    static void drawArrays(int &mode, int &first, int &count);

    static void drawElements(int &mode, int &count, int &type, Buf &indices);

    static void enable(int &cap);

    static void finish();

    static void flush();

    static void frontFace(int &mode);

    static int genTexture();

    static int getError();

    static void getIntegerv(int &pname, std::vector<int> &params);

    static int getInt(int &name);

    static std::string getString(int &name);

    static void hint(int &target, int &mode);

    static void pixelStorei(int &pname, int &param);

    static void polygonOffset(float factor, float units);

    static void readPixels(int &x, int &y, int &width, int &height, int &format, int &type, Buf &pixels);

    static void scissor(int &x, int &y, int &width, int &height);

    static void stencilFunc(int &func, int &ref, int &mask);

    static void stencilMask(int &mask);

    static void stencilOp(int &fail, int &zfail, int &zpass);

    static void texImage2D(int &target, int &level, int &internalformat, int &width, int &height, int &border,
                           int &format, int &type, Buf &pixels);

    static void texParameterf(int &target, int &pname, float &param);

    static void texSubImage2D(int &target, int &level, int &xoffset, int &yoffset, int &width, int &height,
                              int &format, int &type, Buf& pixels);

    static void viewport(int &x, int &y, int &width, int &height);

    static void attachShader(int &program ,int &shader);

    static void bindAttribLocation(int &program, int &index, std::string &name);

    static void bindBuffer(int &target, int &buffer);

    static void bindFramebuffer(int &target, int &framebuffer);

    static void bindRenderbuffer(int &target, int &renderbuffer);

    static void blendColor(float &red, float &green, float &blue, float &alpha);

    static void blendEquation(int &mode);

    static void blendEquationSeparate(int &modeRGB, int &modeAlpha);

    static void blendFuncSeparate(int &srcRGB, int &dstRGB, int &srcAlpha, int &dstAlpha);

    static void bufferData(int &target, int &size, Buf& data, int &usage);

    static void bufferSubData(int &target, int &offset, int &size, Buf &data);

    static int checkFramebufferStatus(int &target);

    static void compileShader(int &shader);

    static int createProgram();

    static int createShader(int &type);

    static void deleteBuffer(int &buffer);

    static void deleteFramebuffer(int &framebuffer);

    static void deleteProgram(int &program);

    static void deleteRenderbuffer(int &renderbuffer);

    static void deleteShader(int &shader);

    static void detachShader(int &program, int &shader);

    static void disableVertexAttribArray(int &index);

    static void drawElements(int &mode, int &count, int &type, int &indices);

    static void enableVertexAttribArray(int &index);

    static void framebufferRenderbuffer(int &target, int &attachment, int &renderbuffertarget, int &renderbuffer);

    static void framebufferTexture2D(int &target, int &attachment, int &textarget, int &texture, int &level);

    static int genBuffer();

    static void generateMipmap(int &target);

    static int genFramebuffer();

    static int genRenderbuffer();

    static std::string getActiveAttrib(int &program, int &index, std::vector<int> &size, std::vector<int> &type);

    static std::string getActiveUniform(int &program, int &index, std::vector<int> &size, std::vector<int> &type);

    static int getAttribLocation(int program, std::string name);

    static void getBooleanv(int &pname, Buf& params);

    static void getBufferParameteriv(int &target, int &pname, std::vector<int> &params);

    static void getFloatv(int &pname, std::vector<float> &params);

    static void getFramebufferAttachmentParameteriv(int &target, int &attachment, int &pname, std::vector<int> &params);

    static void getProgramiv(int &program, int pname, std::vector<int> &params);

    static std::string getProgramInfoLog(int &program);

    static void getRenderbufferParameteriv(int &target, int &pname, std::vector<int> &params);

    static void getShaderiv(int &shader, int pname, std::vector<int> &params);

    static std::string getShaderInfoLog(int &shader);

    static void getShaderPrecisionFormat(int &shadertype, int &precisiontype, std::vector<int> &range, std::vector<int> &precision);

    static void getTexParameterfv(int &target, int &pname, std::vector<float> &params);

    static void getTexParameteriv(int &target, int &pname, std::vector<int> &params);

    static void getUniformfv(int &program, int &location, std::vector<float> &params);

    static void getUniformiv(int &program, int &location, std::vector<int> &params);

    static int getUniformLocation(int &program, std::string &name);

    static void getVertexAttribfv(int &index, int &pname, std::vector<float> &params);

    static void getVertexAttribiv(int &index, int &pname, std::vector<int> &params);

    static bool isBuffer(int &buffer);

    static bool isEnabled(int &cap);

    static bool isFramebuffer(int &framebuffers);

    static bool isProgram(int &program);

    static bool isRenderbuffer(int &renderbuffer);

    static bool isShader(int &shader);

    static bool isTexture(int &texture);

    static void linkProgram(int &program);

    static void releaseShaderCompiler();

    static void renderbufferStorage(int &target, int &internalformat, int &width, int &height);

    static void sampleCoverage(float &value, bool &invert);

    static void shaderSource(int &shader, std::string &string);

    static void stencilFuncSeparate(int &face, int &func, int &ref, int &mask);

    static void stencilMaskSeparate(int &face, int &mask);

    static void stencilOpSeparate(int &face, int &fail, int &zfail, int &zpass);

    static void texParameterfv(int &target, int &pname, std::vector<float> &params);

    static void texParameteri(int &target, int &pname, int &param);

    static void texParameteriv(int &target, int &pname, std::vector<int> &params);

    static void uniform1f(int &location, float &x);

    static void uniform1fv(int &location, int &count, std::vector<float> &v);

    static void uniform1fv(int &location, int count, std::vector<float> &v, int offset);

    static void uniform1i(int &location, int &x);

    static void uniform1iv(int &location, int &count, std::vector<int> &v);

    static void uniform1iv(int &location, int &count, std::vector<int> &v, int &offset);

    static void uniform2f(int &location, float &x, float &y);

    static void uniform2fv(int &location, int &count, std::vector<float> &v);

    static void uniform2fv(int &location, int count, std::vector<float> &v, int offset);

    static void uniform2i(int &location, int &x, int &y);

    static void uniform2iv(int &location, int &count, std::vector<int> &v);

    static void uniform2iv(int &location, int &count, std::vector<int> &v, int &offset);

    static void uniform3f(int &location, float &x, float &y, float &z);

    static void uniform3fv(int &location, int &count, std::vector<float> &v);

    static void uniform3fv(int &location, int count, std::vector<float> &v, int offset);

    static void uniform3i(int &location, int &x, int &y, int &z);

    static void uniform3iv(int &location, int &count, std::vector<int> &v);

    static void uniform3iv(int &location, int &count, std::vector<int> &v, int &offset);

    static void uniform4f(int &location, float &x, float &y, float &z, float &w);

    static void uniform4fv(int &location, int &count, std::vector<float> &v);

    static void uniform4fv(int &location, int count, std::vector<float> &v, int offset);

    static void uniform4i(int &location, int &x, int &y, int &z, int &w);

    static void uniform4iv(int &location, int &count, std::vector<int> &v);

    static void uniform4iv(int &location, int &count, std::vector<int> &v, int &offset);

    static void uniformMatrix2fv(int &location, int &count, bool &transpose, std::vector<float> &value);

    static void uniformMatrix2fv(int &location, int &count, bool &transpose, std::vector<float> &value, int &offset);

    static void uniformMatrix3fv(int &location, int count, bool& transpose, std::vector<float> value);

    static void uniformMatrix3fv(int &location, int count, bool& transpose, std::vector<float> value, int offset);

    static void uniformMatrix4fv(int location, int count, bool transpose, std::vector<float> value);

    static void uniformMatrix4fv(int location, int count, bool transpose, std::vector<float> value, int offset);

    static void useProgram(int program);

    static void validateProgram(int &program);

    static void vertexAttrib1f(int &indx, float &x);

    static void vertexAttrib1fv(int &indx, std::vector<float> &values);

    static void vertexAttrib2f(int &indx, float &x, float &y);

    static void vertexAttrib2fv(int &indx, std::vector<float> &values);

    static void vertexAttrib3f(int &indx, float &x, float &y, float &z);

    static void vertexAttrib3fv(int &indx, std::vector<float> &values);

    static void vertexAttrib4f(int &indx, float &x, float &y, float &z, float &w);

    static void vertexAttrib4fv(int &indx, std::vector<float> &values);

    static void vertexAttribPointer(int &indx, int &size, int &type, bool &normalized, int &stride, Buf &ptr);

    static void vertexAttribPointer(int &indx, int &size, int &type, bool &normalized, int &stride, int &ptr);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_H
