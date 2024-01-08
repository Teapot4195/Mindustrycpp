//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_G2D_TEXTUREREGION_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_G2D_TEXTUREREGION_H

#include <memory>
#include <vector>

// Forward decl to avoid circular reference
class Texture;

class TextureRegion {
public:
    std::shared_ptr<Texture> texture;

    float u{}, v{}, u2{}, v2{};
    int width{}, height{};
    float scale {1};

    TextureRegion();

    TextureRegion(const std::shared_ptr<Texture>& tex);

    TextureRegion(const std::shared_ptr<Texture>& tex, int width, int height);

    static std::vector<std::vector<std::shared_ptr<TextureRegion>>> split(const std::shared_ptr<Texture>& tex, int tileWidth, int tileHeight);


};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_G2D_TEXTUREREGION_H
