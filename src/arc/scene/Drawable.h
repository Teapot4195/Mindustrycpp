//
// Created by teapot on 10/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_SCENE_DRAWABLE_H
#define MINDUSTRY_SRC_ARC_SCENE_DRAWABLE_H


class Drawable {
public:
    virtual ~Drawable() = default;

    virtual void draw(float x, float y, float width, float height) = 0;

    virtual void draw(float x, float y, float originX, float originY, float width, float height, float scaleX, float scaleY, float rotation) = 0;

    virtual float getLeftWidth() = 0;

    virtual void setLeftWidth(float leftWidth) = 0;

    virtual float getRightWidth() = 0;

    virtual void setRightWidth(float rightWidth) = 0;

    virtual float getTopHeight() = 0;

    virtual void setTopHeight(float topHeight) = 0;

    virtual float getBottomHeight() = 0;

    virtual void setBottomHeight(float bottomHeight) = 0;

    virtual float getMinWidth() = 0;

    virtual void setMinWidth(float minWidth) = 0;

    virtual float getMinHeight() = 0;

    virtual void setMinHeight(float minHeight) = 0;

    virtual float imageSize() {
        return getMinWidth();
    }
};


#endif //MINDUSTRY_SRC_ARC_SCENE_DRAWABLE_H
