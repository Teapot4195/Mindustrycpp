//
// Created by teapot on 10/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_G2D_TEXTUREATLAS_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_G2D_TEXTUREATLAS_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <arc/struct.h>
#include <arc/graphics/g2d/TextureRegion.h>
#include <arc/Utilities.h>

// Forward decl to avoid circular reference
class Drawable;
class Fi;
class Pixmap;
class PixmapRegion;
class Texture;
class TextureFilter;
class TextureWrap;

struct AtlasPage {
    const std::shared_ptr<Fi> textureFile;
    const int width, height;
    const bool useMipMaps;
    const std::shared_ptr<TextureFilter> minFilter, magFilter;
    const std::shared_ptr<TextureWrap> uWrap, vWrap;
    std::shared_ptr<Texture> texture;
};

struct Region {
    std::shared_ptr<AtlasPage> page;
    std::string name;
    float offsetX;
    float offsetY;
    int originalWidth;
    int originalHeight;
    bool rotate;
    int left;
    int top;
    int width;
    int height;
    bool flip;
    std::vector<int> splits;
    std::vector<int> pads;
};

class TextureAtlasData {
public:
    static const unsigned char formatVersion = 0;
    constexpr static const unsigned char formatHeader[] = {'A', 'A', 'T', 'L', 'S', '\0'};

    //TODO: These are bare structs, perhaps we don't need to use shared ptrs with them.
    Seq<std::shared_ptr<AtlasPage>> pages;
    Seq<std::shared_ptr<Region>> regions;

    TextureAtlasData(std::shared_ptr<Fi> packFile, std::shared_ptr<Fi> imagesDir, bool flip);

    [[nodiscard]] Seq<std::shared_ptr<AtlasPage>> getPages() const;

    [[nodiscard]] Seq<std::shared_ptr<Region>> getRegions() const;
};

class AtlasRegion : public TextureRegion, public Stringable {
public:
    std::shared_ptr<PixmapRegion> pixmapRegion;

    std::string name;

    float offsetX{};
    float offsetY{};

    int packedWidth;
    int packedHeight;
    int originalWidth;
    int originalHeight;
    bool rotate{};
    std::vector<int> splits;
    std::vector<int> pads;

    AtlasRegion(const std::shared_ptr<Texture>& texture, int x, int y, int width, int height);

    AtlasRegion();

    explicit AtlasRegion(const std::shared_ptr<AtlasRegion>& region);

    explicit AtlasRegion(const std::shared_ptr<TextureRegion>& region);

    void flip(bool x, bool y) override;

    float getRotatePackedWidth() const;

    float getRotatePackedHeight() const;

    std::string toString() override;
};

class TextureAtlas {
private:
    Seq<std::shared_ptr<Texture>> textures;
    Seq<std::shared_ptr<AtlasRegion>> regions;
    ObjectMap<std::string, std::shared_ptr<Drawable>> drawables;
    ObjectMap<std::string, std::shared_ptr<AtlasRegion>> regionmap;
    ObjectMap<std::shared_ptr<Texture>, std::shared_ptr<Pixmap>> pixmaps;

protected:
    std::shared_ptr<AtlasRegion> error, white_;
    float drawableScale = 1;

public:
    static std::shared_ptr<TextureAtlas> blankAtlas();

    TextureAtlas();

    explicit TextureAtlas(std::string internalPackFile);

    explicit TextureAtlas(std::shared_ptr<Fi> packFile);

    TextureAtlas(const std::shared_ptr<Fi>& packFile, bool flip);

    TextureAtlas(const std::shared_ptr<Fi>& packFile, const std::shared_ptr<Fi>& imagesDir);

    TextureAtlas(const std::shared_ptr<Fi>& packFile, const std::shared_ptr<Fi>& imagesDir, bool flip);

    explicit TextureAtlas(std::shared_ptr<TextureAtlasData> data);

    void setDrawableScale(float scale);

    std::shared_ptr<PixmapRegion> getPixmap(std::string name);

    std::shared_ptr<PixmapRegion> getPixmap(std::shared_ptr<AtlasRegion> region);

    ObjectMap<std::shared_ptr<Texture>, std::shared_ptr<Pixmap>> getPixmaps();

    void disposePixmap(std::shared_ptr<Texture> texture);

    std::shared_ptr<PixmapRegion> getPixmap(std::shared_ptr<TextureRegion> region);

    std::shared_ptr<AtlasRegion> addRegion(std::string name, std::shared_ptr<Texture> texture, int x, int y, int width, int height);

    std::shared_ptr<AtlasRegion> addRegion(std::string name, std::shared_ptr<TextureRegion> textureRegion);

    Seq<std::shared_ptr<AtlasRegion>> getRegions() const;

    ObjectMap<std::string, std::shared_ptr<AtlasRegion>> getRegionMap() const;

    std::shared_ptr<AtlasRegion> white();

    bool setErrorRegion(const std::string& name);

    bool isFound(const std::shared_ptr<TextureRegion>& region);

    std::shared_ptr<AtlasRegion> find(std::string name);

    std::shared_ptr<TextureRegion> find(std::string name, std::string def);

    std::shared_ptr<TextureRegion> find(const std::string& name, std::shared_ptr<TextureRegion> def);

    bool has(std::string s) const;

    template <typename T>
    std::shared_ptr<T> getDrawable(std::string name);

    std::shared_ptr<Drawable> drawable(std::string name);

private:
    void load(std::shared_ptr<TextureAtlasData> data);
};

template<typename T>
std::shared_ptr<T> TextureAtlas::getDrawable(std::string name) {
    return std::dynamic_pointer_cast<T>(drawable(std::move(name)));
}


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_G2D_TEXTUREATLAS_H
