//
// Created by teapot on 28/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_TEXTURE_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_TEXTURE_H

#include <memory>
#include <string>
#include <vector>

#include <arc/graphics/GL20.h>
#include <arc/graphics/GLTexture.h>
#include <arc/Utilities.h>

// Forward decl to avoid circular reference
class TextureData;
class Fi;
class Pixmap;

class TextureFilter {
public:
    enum val {
        nearest = GL20::GL_NEAREST,
        linear = GL20::GL_LINEAR,
        mipMap = GL20::GL_LINEAR_MIPMAP_LINEAR,
        mipMapNearestNearest = GL20::GL_NEAREST_MIPMAP_NEAREST,
        mipMapLinearNearest = GL20::GL_LINEAR_MIPMAP_NEAREST,
        mipMapNearestLinear = GL20::GL_NEAREST_MIPMAP_LINEAR,
        mipMapLinearLinear = GL20::GL_LINEAR_MIPMAP_LINEAR,
    };

    const int glEnum;

    static std::vector<std::shared_ptr<TextureFilter>> all;

    explicit TextureFilter(val e);

    [[nodiscard]] bool isMipMap() const;
};

class TextureWrap {
public:
    enum val {
        mirroredRepeat = GL20::GL_MIRRORED_REPEAT,
        clampToEdge = GL20::GL_CLAMP_TO_EDGE,
        repeat = GL20::GL_REPEAT
    };

    static std::vector<std::shared_ptr<TextureWrap>> all;

    const int glEnum;

    explicit TextureWrap(int glEnum);

    [[nodiscard]] int getGLEnum() const;
};

class Texture : public GLTexture, public Stringable {
public:
    std::shared_ptr<TextureData> data;

    explicit Texture(std::string internalPath);

    explicit Texture(std::shared_ptr<Fi> file);

    Texture(std::shared_ptr<Fi> file, bool useMipMaps);

    explicit Texture(const std::shared_ptr<Pixmap>& pixmap);

    Texture(const std::shared_ptr<Pixmap>& pixmap, bool useMipMaps);

    Texture(std::uint32_t width, std::uint32_t height);

    Texture(const std::shared_ptr<TextureData>& data);

protected:
    Texture();

    Texture(int glTarget, int glHandle, const std::shared_ptr<TextureData>& data);

public:
    static std::shared_ptr<Texture> createEmpty(std::shared_ptr<TextureData> data);

    void load(const std::shared_ptr<TextureData>& dat);

    void draw(const std::shared_ptr<Pixmap>& pixmap);

    void draw(const std::shared_ptr<Pixmap>& pixmap, int x, int y);

    int getDpeth() override;

    [[nodiscard]] std::shared_ptr<TextureData> getTextureData() const;

    bool isDisposed() override;

    std::string toString() override;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_TEXTURE_H
