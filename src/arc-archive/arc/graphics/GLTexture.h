//
// Created by teapot on 28/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GLTEXTURE_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GLTEXTURE_H

#include <memory>

#include <util/Disposable.h>

// Forward decl, for enum class C++11+
class TextureFilter;
class TextureWrap;
class TextureData;

class GLTexture : public Disposable {
public:
    const int glTarget;
    int width{}, height{};

protected:
    int glHandle;
    std::shared_ptr<TextureFilter> minFilter, magFilter;
    std::shared_ptr<TextureWrap> uWrap, vWrap;

public:
    explicit GLTexture(int glTarget);

    GLTexture(int glTarget, int glHandle);

    static void uploadImageData(int target, const std::shared_ptr<TextureData>& data, int miplevel);

protected:
    static void uploadImageData(int target, const std::shared_ptr<TextureData> &data);

public:
    virtual int getDpeth() = 0;

    void bind() const;

    void bind(int unit) const;

    std::shared_ptr<TextureFilter> getMinFilter();

    std::shared_ptr<TextureFilter> getMagFilter();

    std::shared_ptr<TextureWrap> getUWrap();

    std::shared_ptr<TextureWrap> getVWrap();

    [[nodiscard]] int getTextureObjectHandle() const;

    void unsafeSetWrap(const std::shared_ptr<TextureWrap>& u, const std::shared_ptr<TextureWrap>& v);

    void unsafeSetWrap(const std::shared_ptr<TextureWrap>& u, const std::shared_ptr<TextureWrap>& v, bool force);

    void setWrap(const std::shared_ptr<TextureWrap>& wrap);

    void setWrap(const std::shared_ptr<TextureWrap>& u, const std::shared_ptr<TextureWrap>& v);

    void unsafeSetFilter(const std::shared_ptr<TextureFilter>& minFilter, const std::shared_ptr<TextureFilter>& magFilter);

    void unsafeSetFilter(const std::shared_ptr<TextureFilter>& minFilter, const std::shared_ptr<TextureFilter>& magFilter, bool force);

    void setFilter(const std::shared_ptr<TextureFilter>& filter);

    void setFilter(const std::shared_ptr<TextureFilter>& min, const std::shared_ptr<TextureFilter>& mag);

    void dispose() override;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GLTEXTURE_H
