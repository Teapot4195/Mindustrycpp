//
// Created by teapot on 10/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_SHADER_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_SHADER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <arc/util/Disposable.h>

// Forward decl to avoid circular reference
class Fi;
class Mat;
class Vec2;
class Vec3;
class Color;

class Shader : public Disposable {
public:
    constexpr static const std::string positionAttribute = "a_position";
    constexpr static const std::string normalAttribute = "a_normal";
    constexpr static const std::string colorAttribute = "a_color";
    constexpr static const std::string mixColorAttribute = "a_mix_color";
    constexpr static const std::string texcoordAttribute = "a_texCoord";

    static bool pedantic;

    static std::string prependVertexCode;

    static std::string prependFragmentCode;

private:
    static std::unordered_map<std::string, int> uniforms;
    static std::unordered_map<std::string, int> uniformTypes;
    static std::unordered_map<std::string, int> uniformSizes;
    static std::unordered_map<std::string, int> attributes;
    static std::unordered_map<std::string, int> attributeTypes;
    static std::unordered_map<std::string, int> attributeSizes;
    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    std::vector<int> params;
    std::vector<int> type;

    std::string log;
    bool isCompiled_{};
    std::vector<std::string> uniformNames;
    std::vector<std::string> attributeNames;
    int program{};
    int vertexShaderHandle{};
    int fragmentShaderHandle{};
    bool disposed{};

    static float val[16];

public:
    Shader(std::string vertexShader, std::string fragmentShader);

    Shader(const std::shared_ptr<Fi>& vertexShader, const std::shared_ptr<Fi>& fragmentShader);

    virtual void apply();

    std::string getLog();

    [[nodiscard]] bool isCompiled() const;

    int fetchUniformLocation(std::string name, bool pedantic_);

    void setUniformi(std::string name, int value);

    void setUniformi(int location, int value);

    void setUniformi(std::string name, int value1, int value2);

    void setUniformi(int location, int value1, int value2);

    void setUniformi(std::string name, int value1, int value2, int value3);

    void setUniformi(int location, int value1, int value2, int value3);

    void setUniformi(std::string name, int value1, int value2, int value3, int value4);

    void setUniformi(int location, int value1, int value2, int value3, int value4);

    void setUniformf(std::string name, float value);

    void setUniformf(int location, float value);

    void setUniformf(std::string name, float value1, float value2);

    void setUniformf(int location, float value1, float value2);

    void setUniformf(std::string name, float value1, float value2, float value3);

    void setUniformf(int location, float value1, float value2, float value3);

    void setUniformf(std::string name, float value1, float value2, float value3, float value4);

    void setUniformf(int location, float value1, float value2, float value3, float value4);

    void setUniform1fv(std::string name, std::vector<float> values, int offset, int length);

    void setUniform1fv(int location, std::vector<float> values, int offset, int length);

    void setUniform2fv(std::string name, std::vector<float> values, int offset, int length);

    void setUniform2fv(int location, std::vector<float> values, int offset, int length);

    void setUniform3fv(std::string name, std::vector<float> values, int offset, int length);

    void setUniform3fv(int location, std::vector<float> values, int offset, int length);

    void setUniform4fv(std::string name, std::vector<float> values, int offset, int length);

    void setUniform4fv(int location, std::vector<float> values, int offset, int length);

    void setUniformMatrix(std::string name, const std::shared_ptr<Mat>& matrix);

    void setUniformMatrix(std::string name, const std::shared_ptr<Mat>& matrix, bool transpose);

    void setUniformMatrix(int location, const std::shared_ptr<Mat>& matrix);

    void setUniformMatrix(int location, const std::shared_ptr<Mat>& matrix, bool transpose);

    void setUniformMatrix4(std::string name, std::vector<float> matrix, bool transpose);

    void setUniformMatrix4(std::string name, const std::shared_ptr<Mat>& matrix);

    void setUniformMatrix4(std::string name, const std::shared_ptr<Mat>& matrix, float near, float far);

    void setUniformMatrix3fv(std::string name, std::vector<float> buffer, int count, bool transpose);

    void setUniformMatrix4fv(std::string name, std::vector<float> buffer, int count, bool transpose);

    void setUniformMatrix4fv(int location, std::vector<float> values, int offset, int length);

    void setUniformMatrix4fv(std::string name, std::vector<float> values, int offset, int length);

    void setUniformf(std::string name, const std::shared_ptr<Vec2>& values);

    void setUniformf(int location, const std::shared_ptr<Vec2>& values);

    void setUniformf(std::string name, const std::shared_ptr<Vec3>& values);

    void setUniformf(int name, const std::shared_ptr<Vec3>& values);

    void setUniformf(std::string name, const std::shared_ptr<Color>& values);

    void setUniformf(int location, const std::shared_ptr<Color>& values);

    void bind() const;

    void dispose() override;

    bool isDisposed() override;

    void disableVertexAttribute(const std::string& name);

    bool hasAttribute(const std::string& name);

    int getAttributeType(const std::string& name);

    int getAttributeLocation(const std::string& name);

    int getAttributeSize(const std::string& name);

    bool hasUniform(const std::string& name);

    int getUniformType(const std::string& name);

    int getUniformLocation(const std::string& name);

    static static int getUniformSize(const std::string& name);

    std::vector<std::string> getAttributes();

    std::vector<std::string> getUniforms();

    std::string getVertexShaderSource();

    std::string getFragmentShaderSource();

    // Mistakes were made
    static std::vector<float> copyTransform(const std::shared_ptr<Mat>& matrix);

    static std::vector<float> copyTransform(const std::shared_ptr<Mat>& matrix, float near, float far);

protected:
    std::string preprocess(std::string source, bool fragment);

    int createProgram();

private:
    void compileShaders(std::string vertexShader, std::string fragmentShader);

    int loadShader(int type_, std::string source);

    int linkProgram(int prog);

    [[nodiscard]] int fetchAttributeLocation(const std::string& name) const;

    int fetchUniformLocation(std::string name);

    void fetchUniform();

    void fetchAttributes();
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_SHADER_H
