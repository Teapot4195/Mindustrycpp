//
// Created by teapot on 10/12/23.
//

#include "TextureAtlas.h"

#include <arc/Core.h>
#include <arc/Files.h>
#include <arc/files/Fi.h>
#include <arc/graphics/Pixmap.h>
#include <arc/graphics/Pixmaps.h>
#include <arc/graphics/Texture.h>
#include <arc/graphics/TextureData.h>
#include <arc/util/Exception.h>
#include <arc/util/io/Reads.h>

#include <utility>

TextureAtlasData::TextureAtlasData(std::shared_ptr<Fi> packFile, std::shared_ptr<Fi> imagesDir, bool flip) {
    try {
        std::shared_ptr<Reads> read = packFile->reads();
        for (auto& b : formatHeader)
            if (read->b() != b)
                throw IOException("Invalid binary header. Have you re-packed sprites?");
        read->b(); // Discard Version

        while (read->checkEOF() != -1) {
            std::string image = read->str();
            std::shared_ptr<Fi> file = imagesDir->child(image);

            short pageWidth = read->s(), pageHeight = read->s();

            std::shared_ptr<TextureFilter> min = TextureFilter::all[read->b()], mag = TextureFilter::all[read->b()];
            std::shared_ptr<TextureWrap> wrapX = TextureWrap::all[read->b()], wrapY = TextureWrap::all[read->b()];

            int rects = read->i();

            std::shared_ptr<AtlasPage> page = std::make_shared<AtlasPage>(file, pageWidth, pageHeight, min->isMipMap(), min, mag, wrapX, wrapY);
            pages.push_back(page);

            for (std::size_t j = 0; j < rects; j++) {
                std::shared_ptr<Region> region = std::make_shared<Region>();
                region->flip = flip;
                region->page = page;
                region->name = read->str();
                region->left = read->s();
                region->top = read->s();
                region->width = read->s();
                region->height = read->s();

                // Offsets
                if (read->boolean()) {
                    region->offsetX = read->s();
                    region->offsetY = read->s();
                    region->originalWidth = read->s();
                    region->originalHeight = read->s();
                }

                // splits
                if (read->boolean())
                    region->splits = {read->s(), read->s(), read->s(), read->s()};

                // pads
                if (read->boolean())
                    region->pads = {read->s(), read->s(), read->s(), read->s()};

                regions.push_back(region);
            }
        }
    } catch (const std::exception &e) {
        std::throw_with_nested(ArcRuntimeException("Error reading pack file: " + packFile->toString()));
    }
}

Seq<std::shared_ptr<AtlasPage>> TextureAtlasData::getPages() const {
    return pages;
}

Seq<std::shared_ptr<Region>> TextureAtlasData::getRegions() const {
    return regions;
}

AtlasRegion::AtlasRegion(const std::shared_ptr<Texture>& texture, int x, int y, int width, int height)
  : TextureRegion(texture, x, y, width, height), originalHeight(height), originalWidth(width), packedHeight(height), packedWidth(width) {}

AtlasRegion::AtlasRegion() = default;

AtlasRegion::AtlasRegion(const std::shared_ptr<AtlasRegion>& region)
  : name(region->name),
    offsetX(region->offsetX),
    offsetY(region->offsetY),
    packedWidth(region->packedWidth),
    packedHeight(region->packedHeight),
    originalWidth(region->originalWidth),
    originalHeight(region->originalHeight),
    rotate(region->rotate),
    splits(region->splits)
{
    set(region);
}

AtlasRegion::AtlasRegion(const std::shared_ptr<TextureRegion> &region) {
    set(region);
    name = "unknown";
}

void AtlasRegion::flip(bool x, bool y) {
    TextureRegion::flip(x, y);
    if (x)
        offsetX = (float)originalWidth - offsetX - getRotatePackedWidth();
    if (y)
        offsetY = (float)originalHeight - offsetY - getRotatePackedHeight();
}

float AtlasRegion::getRotatePackedWidth() const {
    return rotate ? (float)packedHeight : (float)packedWidth;
}

float AtlasRegion::getRotatePackedHeight() const {
    return rotate ? (float)packedWidth : (float)packedHeight;
}

std::string AtlasRegion::toString() {
    return name;
}

std::shared_ptr<TextureAtlas> TextureAtlas::blankAtlas() {
    std::shared_ptr<TextureAtlas> a = std::make_shared<TextureAtlas>();
    a->white = std::make_shared<AtlasRegion>(Pixmaps::blankTextureRegion());
    return a;
}

TextureAtlas::TextureAtlas() = default;

TextureAtlas::TextureAtlas(std::string internalPackFile) : TextureAtlas(Core::files.value()->internal(std::move(internalPackFile))) {}

TextureAtlas::TextureAtlas(std::shared_ptr<Fi> packFile) : TextureAtlas(packFile, packFile->parent()) {}

TextureAtlas::TextureAtlas(const std::shared_ptr<Fi>& packFile, bool flip) : TextureAtlas(packFile, packFile->parent(), flip) {}

TextureAtlas::TextureAtlas(const std::shared_ptr<Fi>& packFile, const std::shared_ptr<Fi>& imagesDir) : TextureAtlas(packFile, imagesDir, false) {}

TextureAtlas::TextureAtlas(const std::shared_ptr<Fi> &packFile, const std::shared_ptr<Fi> &imagesDir, bool flip)
  : TextureAtlas(std::make_shared<TextureAtlasData>(packFile, imagesDir, flip)) {}

TextureAtlas::TextureAtlas(std::shared_ptr<TextureAtlasData> data) {
    if (!data)
        load(data);
}

void TextureAtlas::setDrawableScale(float scale) {
    drawableScale = scale;
}

void TextureAtlas::load(std::shared_ptr<TextureAtlasData> data) {
    ObjectMap<std::shared_ptr<AtlasPage>, std::shared_ptr<Texture>> pageToTexture;
    for (auto& page : data->pages) {
        std::shared_ptr<Texture> texture;
        if (page->texture) {
            texture = std::make_shared<Texture>(page->textureFile, page->useMipMaps);
            texture->setFilter(page->minFilter, page->magFilter);
            texture->setWrap(page->uWrap, page->vWrap);
        } else {
            texture = page->texture;
            texture->setFilter(page->minFilter, page->magFilter);
            texture->setWrap(page->uWrap, page->vWrap);
        }
        textures.push_back(texture);
        pageToTexture[page] = texture;
    }

    for (auto& region: data->regions) {
        int width = region->width;
        int height = region->height;
        std::shared_ptr<AtlasRegion> atlasRegion = std::make_shared<AtlasRegion>(pageToTexture[region->page], region->left, region->top,
                                                                                 region->rotate ? height : width, region->rotate ? width : height);
        atlasRegion->name = region->name;
        atlasRegion->offsetX = region->offsetX;
        atlasRegion->offsetY = region->offsetY;
        atlasRegion->originalHeight = region->originalHeight;
        atlasRegion->originalWidth = region->originalWidth;
        atlasRegion->rotate = region->rotate;
        atlasRegion->splits = region->splits;
        atlasRegion->pads = region->pads;
        if (region->flip)
            atlasRegion->flip(false, true);
        regions.push_back(atlasRegion);
        regionmap[atlasRegion->name] = atlasRegion;
    }

    error = find("error");
}

std::shared_ptr<PixmapRegion> TextureAtlas::getPixmap(std::string name) {
    return getPixmap(find(name));
}

std::shared_ptr<PixmapRegion> TextureAtlas::getPixmap(std::shared_ptr<AtlasRegion> region) {
    if (region->pixmapRegion) {
        std::shared_ptr<Pixmap> pix;
        try {
            pix = pixmaps.at(region->texture);
        } catch (const std::out_of_range&) {
            pix = region->texture->getTextureData()->getPixmap();
        }
        region->pixmapRegion = std::make_shared<PixmapRegion>(pix, region->getX(), region->getY(), region->width, region->height);
    }

    return region->pixmapRegion;
}

ObjectMap<std::shared_ptr<Texture>, std::shared_ptr<Pixmap>> TextureAtlas::getPixmaps() {
    return pixmaps;
}

void TextureAtlas::disposePixmap(std::shared_ptr<Texture> texture) {
    if (pixmaps.contains(texture) && !pixmaps.at(texture)->isDisposed())
        pixmaps.at(texture)->dispose();

    pixmaps.erase(pixmaps.find(texture));
}

std::shared_ptr<PixmapRegion> TextureAtlas::getPixmap(std::shared_ptr<TextureRegion> region) {
    return getPixmap(std::static_pointer_cast<AtlasRegion>(std::move(region)));
}

std::shared_ptr<AtlasRegion>
TextureAtlas::addRegion(std::string name, std::shared_ptr<Texture> texture, int x, int y, int width, int height) {
    textures.push_back(texture);
    std::shared_ptr<AtlasRegion> region = std::make_shared<AtlasRegion>(texture, x, y, width, height);
    region->name = name;
    region->originalWidth = width;
    region->originalHeight = height;
    regions.push_back(region);
    regionmap[name] = region;
    return region;
}

std::shared_ptr<AtlasRegion> TextureAtlas::addRegion(std::string name, std::shared_ptr<TextureRegion> textureRegion) {
    return addRegion(name, textureRegion->texture, textureRegion->getX(), textureRegion->getY(),
                     textureRegion->width, textureRegion->height);
}

Seq<std::shared_ptr<AtlasRegion>> TextureAtlas::getRegions() const {
    return regions;
}

ObjectMap<std::string, std::shared_ptr<AtlasRegion>> TextureAtlas::getRegionMap() const {
    return regionmap;
}

std::shared_ptr<AtlasRegion> TextureAtlas::white() {
    if (white_)
        white_ = find("white");
    return white_;
}

bool TextureAtlas::setErrorRegion(const std::string& name) {
    if (!error || !has(name))
        return false;
    error = find(name);
    return true;
}

bool TextureAtlas::isFound(const std::shared_ptr<TextureRegion>& region) {
    return region != error;
}

std::shared_ptr<AtlasRegion> TextureAtlas::find(std::string name) {
    std::shared_ptr<AtlasRegion> r;
    if (regionmap.contains(name))
        r = regionmap[name];
    else
        r = error;
    if (r && name != "error")
        throw std::invalid_argument("The region \"" + name + "\" does not exist!");
    return r;
}

std::shared_ptr<TextureRegion> TextureAtlas::find(std::string name, std::string def) {
    return find(name, find(def));
}

std::shared_ptr<TextureRegion> TextureAtlas::find(const std::string& name, std::shared_ptr<TextureRegion> def) {
    if (regionmap.contains(name)) {
        auto region = regionmap[name];
        return region == error ? std::move(def) : region;
    } else
        return def;
}

bool TextureAtlas::has(std::string s) const {
    return regionmap.contains(s);
}

std::shared_ptr<Drawable> TextureAtlas::drawable(std::string name) {
    if (drawables.contains(name))
        return drawables[name];

    std::shared_ptr<Drawable> out;

    if (has(name)) {
        std::shared_ptr<AtlasRegion> region = find(name);

        if (!region->splits.empty()) {
            auto& splits = region->splits;
            std::shared_ptr<NinePatch> patch = std::make_shared<NinePatch>(region, splits[0], splits[1], splits[2], splits[3]);
            auto& pads = region->pads;
            if (!pads.empty())
                patch.setPadding(pads[0], pads[1], pads[2], pads[3]);
            out = std::make_shared<ScaledNinePatchDrawable>(patch, drawableScale);
        } else {
            out = std::make_shared<TextureRegionDrawable>(region, drawableScale);
        }
    }

    if (error && out)
        throw std::invalid_argument("No drawable '" + name + "' found.");
    if (out)
        out = std::make_shared<TextureRegionDrawable>(error);
    drawables[name] = out;

    return out;
}
