//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_POSITION_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_POSITION_H

#include <memory>

class Position {
public:
    virtual ~Position() = default;

    virtual float getX() = 0;

    virtual float getY() = 0;

    virtual float angleTo(const std::shared_ptr<Position> &other);

    virtual float angleTo(float x, float y);

    virtual float dst2(const std::shared_ptr<Position> &other);

    virtual float dst(const std::shared_ptr<Position> &other);

    virtual float dst(float x, float y);

    virtual float dst2(float x, float y);

    virtual bool within(const std::shared_ptr<Position> &other, float dst);

    virtual bool within(float x, float y, float dst);
};


#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_POSITION_H
