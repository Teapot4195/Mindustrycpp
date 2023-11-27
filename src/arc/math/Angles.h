//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_ANGLES_H
#define MINDUSTRY_SRC_ARC_MATH_ANGLES_H

#include <memory>

#include <func.h>
#include "Rand.h"
#include "geom/Vec2.h"

class Angles {
private:
    static Rand rand;
    static std::shared_ptr<Vec2> rv;

public:
    using ParticleConsumer = std::function<void(float, float, float, float)>;

    static float forwardDistance(float angle1, float angle2);

    static float backwardDistance(float angle1, float angle2);

    static bool within(float a, float b, float margin);

    static float angleDist(float a, float b);

    static bool near(float a, float b, float range);

    static float clampRange(float angle, float dest, float range);

    static float moveToward(float angle, float to, float speed);

    static float angle(float x, float y);

    static float angle(float x, float y, float x2, float y2);

    static float angleRad(float x, float y, float x2, float y2);

    static float trnsx(float angle, float len);

    static float trnsy(float angle, float len);

    static float trnsx(float angle, float x, float y);

    static float trnsy(float angle, float x, float y);

    static float mouseAngle(float cx, float cy);

    static void circleVectors(std::uint32_t points, float length, const Floatc2& pos);

    static void circleVectors(std::uint32_t points, float length, float offset, const Floatc2& pos);

    static void randVectors(std::uint64_t seed, std::uint32_t amount, float length, const Floatc2& cons);

    static void randLenVectors(std::uint64_t seed, std::uint32_t amount, float length, const Floatc2& cons);

    static void randLenVectors(std::uint64_t seed, std::uint32_t amount, float minLength, float length, const Floatc2& cons);

    static void randLenVectors(std::uint64_t seed, std::uint32_t amount, float length, float angle, float range, const Floatc2& cons);

    static void randLenVectors(std::uint64_t seed, std::uint32_t amount, float length, float angle, float range, float spread, const Floatc2& cons);

    static void randLenVectors(std::uint64_t seed, float fin, std::uint32_t amount, float length, const ParticleConsumer& cons);

    static void randLenVectors(std::uint64_t seed, float fin, std::uint32_t amount, float length , float angle, float range, const ParticleConsumer& cons);
};


#endif //MINDUSTRY_SRC_ARC_MATH_ANGLES_H
