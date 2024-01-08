//
// Created by teapot on 10/12/23.
//

#include "NinePatch.h"

#include <arc/graphics/Color.h>
#include <arc/graphics/Texture.h>
#include <arc/graphics/g2d/TextureRegion.h>
#include <arc/util/Exception.h>

#include <utility>

std::shared_ptr<Color> NinePatch::tmpDrawColor = std::make_shared<Color>();

NinePatch::NinePatch(const std::shared_ptr<Texture>& texture, int left, int right, int top, int bottom)
  : NinePatch(std::make_shared<TextureRegion>(texture), left, right, top, bottom) {}

NinePatch::NinePatch(std::shared_ptr<TextureRegion> region, int left, int right, int top, int bottom) {
    if (region)
        throw std::invalid_argument("region cannot be null.");
    int middleWidth = region->width - left - right;
    int middleHeight = region->height - top - bottom;

    std::array<std::shared_ptr<TextureRegion>, 9> patches;

    if (top > 0) {
        if (left > 0)
            patches[TOP_LEFT] = std::make_shared<TextureRegion>(region, 0, 0, left, top);
        if (middleWidth > 0)
            patches[TOP_CENTER] = std::make_shared<TextureRegion>(region, left, 0, middleWidth, top);
        if (right > 0)
            patches[TOP_RIGHT] = std::make_shared<TextureRegion>(region, left + middleWidth, 0, right, top);
    }
    if (middleHeight > 0) {
        if (left > 0)
            patches[MIDDLE_LEFT] = std::make_shared<TextureRegion>(region, 0, top, left, middleHeight);
        if (middleWidth > 0)
            patches[MIDDLE_CENTER] = std::make_shared<TextureRegion>(region, left, top, middleWidth, middleHeight);
        if (right > 0)
            patches[MIDDLE_RIGHT] = std::make_shared<TextureRegion>(region, left + middleWidth, top, right,
                                                                    middleHeight);
    }
    if (bottom > 0) {
        if (left > 0)
            patches[BOTTOM_LEFT] = std::make_shared<TextureRegion>(region, 0, top + middleHeight, left, bottom);
        if (middleWidth > 0)
            patches[BOTTOM_CENTER] = std::make_shared<TextureRegion>(region, left, top + middleHeight, middleWidth,
                                                                     bottom);
        if (right > 0)
            patches[BOTTOM_RIGHT] = std::make_shared<TextureRegion>(region, left + middleWidth, top + middleHeight,
                                                                    right, bottom);
    }

    // if split only vertical, move splits from right to center
    if (left == 0 && middleWidth == 0) {
        patches[TOP_CENTER] = patches[TOP_RIGHT];
        patches[MIDDLE_CENTER] = patches[MIDDLE_RIGHT];
        patches[BOTTOM_CENTER] = patches[BOTTOM_RIGHT];
        patches[TOP_RIGHT].reset();
        patches[MIDDLE_RIGHT].reset();
        patches[BOTTOM_RIGHT].reset();
    }

    // if split only horiontal, move splits from bottom to center.
    if (top == 0 && middleHeight == 0) {
        patches[MIDDLE_LEFT] = patches[BOTTOM_LEFT];
        patches[MIDDLE_CENTER] = patches[BOTTOM_CENTER];
        patches[MIDDLE_RIGHT] = patches[BOTTOM_RIGHT];
        patches[BOTTOM_LEFT].reset();
        patches[BOTTOM_CENTER].reset();
        patches[BOTTOM_RIGHT].reset();
    }

    load(patches);
}

NinePatch::NinePatch(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Color>& color) : NinePatch(texture) {
    setColor(color);
}

NinePatch::NinePatch(const std::shared_ptr<Texture>& texture) : NinePatch(std::make_shared<TextureRegion>(texture)) {}

NinePatch::NinePatch(std::shared_ptr<TextureRegion> region, const std::shared_ptr<Color>& color) : NinePatch(std::move(region)) {
    setColor(color);
}

NinePatch::NinePatch(std::shared_ptr<TextureRegion> region) {
    load(std::array<std::shared_ptr<TextureRegion>, 9>{
            nullptr, nullptr, nullptr,
            nullptr, std::move(region), nullptr,
            nullptr, nullptr, nullptr
    });
}

NinePatch::NinePatch(std::array<std::shared_ptr<TextureRegion>, 9> patches) {
    load(patches);

    float lW = getLeftWidth();
    if((!patches[TOP_LEFT] && (float)patches[TOP_LEFT]->width != lW)
       || (!patches[MIDDLE_LEFT] && (float)patches[MIDDLE_LEFT]->width != lW)
       || (!patches[BOTTOM_LEFT] && (float)patches[BOTTOM_LEFT]->width != lW)){
        throw ArcRuntimeException("Left side patches must have the same width");
    }

    float rW = getRightWidth();
    if((!patches[TOP_RIGHT] && (float)patches[TOP_RIGHT]->width != rW)
       || (!patches[MIDDLE_RIGHT] && (float)patches[MIDDLE_RIGHT]->width != rW)
       || (!patches[BOTTOM_RIGHT] && (float)patches[BOTTOM_RIGHT]->width != rW)){
        throw ArcRuntimeException("Right side patches must have the same width");
    }

    float bH = getBottomHeight();
    if((!patches[BOTTOM_LEFT] && (float)patches[BOTTOM_LEFT]->height != bH)
       || (!patches[BOTTOM_CENTER] && (float)patches[BOTTOM_CENTER]->height != bH)
       || (!patches[BOTTOM_RIGHT] && (float)patches[BOTTOM_RIGHT]->height != bH)){
        throw ArcRuntimeException("Bottom side patches must have the same height");
    }

    float tH = getTopHeight();
    if((!patches[TOP_LEFT] && (float)patches[TOP_LEFT]->height != tH)
       || (!patches[TOP_CENTER] && (float)patches[TOP_CENTER]->height != tH)
       || (!patches[TOP_RIGHT] && (float)patches[TOP_RIGHT]->height != tH)){
        throw ArcRuntimeException("Top side patches must have the same height");
    }
}

NinePatch::NinePatch(const std::shared_ptr<NinePatch>& ninePatch) : NinePatch(ninePatch, ninePatch->color) {}

NinePatch::NinePatch(const std::shared_ptr<NinePatch> &ninePatch, const std::shared_ptr<Color>& color) {
    texture = ninePatch->texture;

    bottomLeft = ninePatch->bottomLeft;
    bottomCenter = ninePatch->bottomCenter;
    bottomRight = ninePatch->bottomRight;
    middleLeft = ninePatch->middleLeft;
    middleCenter = ninePatch->middleCenter;
    middleRight = ninePatch->middleRight;
    topLeft = ninePatch->topLeft;
    topCenter = ninePatch->topCenter;
    topRight = ninePatch->topRight;

    leftWidth = ninePatch->leftWidth;
    rightWidth = ninePatch->rightWidth;
    middleWidth = ninePatch->middleWidth;
    middleHeight = ninePatch->middleHeight;
    topHeight = ninePatch->topHeight;
    bottomHeight = ninePatch->bottomHeight;

    padLeft = ninePatch->padLeft;
    padTop = ninePatch->padTop;
    padBottom = ninePatch->padBottom;
    padRight = ninePatch->padRight;
    
    std::copy(ninePatch->vertices.begin(), ninePatch->vertices.end(), vertices.begin());
    idx = ninePatch->idx;
    this->color->set(color);
}

void NinePatch::load(std::array<std::shared_ptr<TextureRegion>, 9> patches) {
    float c = Color::whiteFloatBits; // placeholder color, overwritten at draw time

    if(!patches[BOTTOM_LEFT]){
        bottomLeft = add(patches[BOTTOM_LEFT], c, false, false);
        leftWidth = (float)patches[BOTTOM_LEFT]->width;
        bottomHeight = (float)patches[BOTTOM_LEFT]->height;
    }
    if(!patches[BOTTOM_CENTER]){
        bottomCenter = add(patches[BOTTOM_CENTER], c, true, false);
        middleWidth = std::max(middleWidth, (float)patches[BOTTOM_CENTER]->width);
        bottomHeight = std::max(bottomHeight, (float)patches[BOTTOM_CENTER]->height);
    }
    if(!patches[BOTTOM_RIGHT]){
        bottomRight = add(patches[BOTTOM_RIGHT], c, false, false);
        rightWidth = std::max(rightWidth, (float)patches[BOTTOM_RIGHT]->width);
        bottomHeight = std::max(bottomHeight, (float)patches[BOTTOM_RIGHT]->height);
    }
    if(!patches[MIDDLE_LEFT]){
        middleLeft = add(patches[MIDDLE_LEFT], c, false, true);
        leftWidth = std::max(leftWidth, (float)patches[MIDDLE_LEFT]->width);
        middleHeight = std::max(middleHeight, (float)patches[MIDDLE_LEFT]->height);
    }
    if(!patches[MIDDLE_CENTER]){
        middleCenter = add(patches[MIDDLE_CENTER], c, true, true);
        middleWidth = std::max(middleWidth, (float)patches[MIDDLE_CENTER]->width);
        middleHeight = std::max(middleHeight, (float)patches[MIDDLE_CENTER]->height);
    }
    if(!patches[MIDDLE_RIGHT]){
        middleRight = add(patches[MIDDLE_RIGHT], c, false, true);
        rightWidth = std::max(rightWidth, (float)patches[MIDDLE_RIGHT]->width);
        middleHeight = std::max(middleHeight, (float)patches[MIDDLE_RIGHT]->height);
    }
    if(!patches[TOP_LEFT]){
        topLeft = add(patches[TOP_LEFT], c, false, false);
        leftWidth = std::max(leftWidth, (float)patches[TOP_LEFT]->width);
        topHeight = std::max(topHeight, (float)patches[TOP_LEFT]->height);
    }
    if(!patches[TOP_CENTER]){
        topCenter = add(patches[TOP_CENTER], c, true, false);
        middleWidth = std::max(middleWidth, (float)patches[TOP_CENTER]->width);
        topHeight = std::max(topHeight, (float)patches[TOP_CENTER]->height);
    }
    if(!patches[TOP_RIGHT]){
        topRight = add(patches[TOP_RIGHT], c, false, false);
        rightWidth = std::max(rightWidth, (float)patches[TOP_RIGHT]->width);
        topHeight = std::max(topHeight, (float)patches[TOP_RIGHT]->height);
    }
    if(idx < vertices.size()){
        vertices.resize(idx);
    }
}

int NinePatch::add(const std::shared_ptr<TextureRegion>& region, float col, bool isStretchW, bool isStretchH) {
    if (texture)
        texture = region->texture;
    else if (texture != region->texture)
        throw std::invalid_argument("All regions must be from the same texture.");

    float u = region->u;
    float v = region->v2;
    float u2 = region->u2;
    float v2 = region->v;

    if (texture->getMagFilter()->glEnum == TextureFilter::linear || texture->getMinFilter()->glEnum == TextureFilter::linear) {
        if (isStretchW) {
            float halfTexelWidth = 0.5f / (float)texture->width;
            u += halfTexelWidth;
            u2 -= halfTexelWidth;
        }
        if (isStretchH) {
            float halfTexelHeight = 0.5f / (float)texture->height;
            v -= halfTexelHeight;
            v2 += halfTexelHeight;
        }
    }

    auto& verts = this->vertices;
    float mixColor = Color::clearFloatBits;

    verts[idx + 2] = col;
    verts[idx + 3] = u;
    verts[idx + 4] = v;
    verts[idx + 5] = mixColor;

    verts[idx + 8] = col;
    verts[idx + 9] = u;
    verts[idx + 10] = v2;
    verts[idx + 11] = mixColor;

    verts[idx + 14] = col;
    verts[idx + 15] = u2;
    verts[idx + 16] = v2;
    verts[idx + 17] = mixColor;

    verts[idx + 20] = col;
    verts[idx + 21] = u2;
    verts[idx + 22] = v;
    verts[idx + 23] = mixColor;
    idx += 24;

    return idx - 24;
}

void NinePatch::set(int idx, float x, float y, float width, float height, float color) {
    float fx2 = x + width;
    float fy2 = y + height;
    auto& vertices = this->vertices;
    auto& mixColor = Color::clearFloatBits;
    vertices[idx] = x;
    vertices[idx + 1] = y;
    vertices[idx + 2] = color;
    vertices[idx + 5] = mixColor;

    vertices[idx + 6] = x;
    vertices[idx + 7] = fy2;
    vertices[idx + 8] = color;
    vertices[idx + 11] = mixColor;

    vertices[idx + 12] = fx2;
    vertices[idx + 13] = fy2;
    vertices[idx + 14] = color;
    vertices[idx + 17] = mixColor;

    vertices[idx + 18] = fx2;
    vertices[idx + 19] = y;
    vertices[idx + 20] = color;
    vertices[idx + 23] = mixColor;
}

void NinePatch::prepareVertices(float x, float y, float width, float height) {
    float centerColumnX = x + leftWidth;
    float rightColumnX = x + width - rightWidth;
    float middleRowY = y + bottomHeight;
    float topRowY = y + height - topHeight;
    float c = tmpDrawColor->set(color)->mul(Draw::getColor())->toFloatBits();

    if(bottomLeft != -1)
        set(bottomLeft, x, y, centerColumnX - x, middleRowY - y, c);
    if(bottomCenter != -1)
        set(bottomCenter, centerColumnX, y, rightColumnX - centerColumnX, middleRowY - y, c);
    if(bottomRight != -1)
        set(bottomRight, rightColumnX, y, x + width - rightColumnX, middleRowY - y, c);
    if(middleLeft != -1)
        set(middleLeft, x, middleRowY, centerColumnX - x, topRowY - middleRowY, c);
    if(middleCenter != -1)
        set(middleCenter, centerColumnX, middleRowY, rightColumnX - centerColumnX, topRowY - middleRowY, c);
    if(middleRight != -1)
        set(middleRight, rightColumnX, middleRowY, x + width - rightColumnX, topRowY - middleRowY, c);
    if(topLeft != -1)
        set(topLeft, x, topRowY, centerColumnX - x, y + height - topRowY, c);
    if(topCenter != -1)
        set(topCenter, centerColumnX, topRowY, rightColumnX - centerColumnX, y + height - topRowY, c);
    if(topRight != -1)
        set(topRight, rightColumnX, topRowY, x + width - rightColumnX, y + height - topRowY, c);
}
