//
// Created by teapot on 10/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_G2D_NINEPATCH_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_G2D_NINEPATCH_H

#include <array>
#include <memory>
#include <vector>

// Forward decl to avoid circular reference
class Color;
class Texture;
class TextureRegion;

class NinePatch {
public:
    enum pos {
        TOP_LEFT = 0,
        TOP_CENTER = 1,
        TOP_RIGHT = 2,
        MIDDLE_LEFT = 3,
        MIDDLE_CENTER = 4,
        MIDDLE_RIGHT = 5,
        BOTTOM_LEFT = 6,
        BOTTOM_CENTER = 7,
        BOTTOM_RIGHT = 8
    };

private:
    static std::shared_ptr<Color> tmpDrawColor;

    std::shared_ptr<Color> color;
    std::shared_ptr<Texture> texture;

    int bottomLeft = -1, bottomCenter = -1, bottomRight = -1;
    int middleLeft = -1, middleCenter = -1, middleRight = -1;
    int topLeft = -1, topCenter = -1, topRight = -1;

    float leftWidth{}, rightWidth{}, middleWidth{}, middleHeight{}, topHeight{}, bottomHeight{};

    std::vector<float> vertices;

    int idx{};

    float padLeft = -1, padRight = -1, padTop = -1, padBottom = -1;

public:
    NinePatch(const std::shared_ptr<Texture>& texture, int left, int right, int top, int bottom);

    NinePatch(std::shared_ptr<TextureRegion> region, int left, int right, int top, int bottom);

    NinePatch(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Color>& color);

    explicit NinePatch(const std::shared_ptr<Texture>& texture);

    NinePatch(std::shared_ptr<TextureRegion> region, const std::shared_ptr<Color>& color);

    explicit NinePatch(std::shared_ptr<TextureRegion> region);

    explicit NinePatch(std::array<std::shared_ptr<TextureRegion>, 9> patches);

    explicit NinePatch(const std::shared_ptr<NinePatch>& ninePatch);

    NinePatch(const std::shared_ptr<NinePatch>& ninePatch, const std::shared_ptr<Color>& color);

private:
    void load(std::array<std::shared_ptr<TextureRegion>, 9> patches);

    int add(const std::shared_ptr<TextureRegion>& region, float col, bool isStretchW, bool isStretchH);

    void set(int idx, float x, float y, float width, float height, float color);

    void prepareVertices(float x, float y, float width, float height);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_G2D_NINEPATCH_H
