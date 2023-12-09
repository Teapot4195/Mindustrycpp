//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_SHAPE2D_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_SHAPE2D_H

#include <memory>

// Forward decl to avoid circular reference
class Vec2;

class Shape2D {
public:
    virtual ~Shape2D() = default;

    virtual bool contains(std::shared_ptr<Vec2> point) = 0;

    virtual bool contains(float x, float y) = 0;
};


#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_SHAPE2D_H
