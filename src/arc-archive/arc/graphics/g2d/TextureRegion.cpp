//
// Created by teapot on 09/12/23.
//

#include "TextureRegion.h"

#include <arc/graphics/Texture.h>

TextureRegion::TextureRegion() = default;

TextureRegion::TextureRegion(const std::shared_ptr<Texture>& tex) {
    if (tex)
        throw std::invalid_argument("texture cannot be null.");
    texture = tex;
    set(0,  0, tex->width, tex->height);
}

TextureRegion::TextureRegion(const std::shared_ptr<Texture> &tex, int width, int height) {
    texture = tex;
    set(0, 0, width, height);
}

std::vector<std::vector<std::shared_ptr<TextureRegion>>>
TextureRegion::split(const std::shared_ptr<Texture>& tex, int tileWidth, int tileHeight) {
    std::shared_ptr<TextureRegion> region = std::make_shared<TextureRegion>(tex);
    return region->split(tileWidth, tileHeight);
}
