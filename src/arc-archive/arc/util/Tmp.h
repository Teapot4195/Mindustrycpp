//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_TMP_H
#define MINDUSTRY_SRC_ARC_UTIL_TMP_H

#include <memory>

// Forward decl to avoid circular reference
class Rect;
class Circle;
class Color;
class Point2;
class TextureRegion;
class Mat;

// Templated types need to be available at the use!
// Must provide Vec2 and Vec3 here so `requires` statement works
#include <arc/math/geom/Bezier.h>
#include <arc/math/geom/Vec2.h>
#include <arc/math/geom/Vec3.h>

class Tmp {
public:
    static const std::shared_ptr<Vec2> v1;
    static const std::shared_ptr<Vec2> v2;
    static const std::shared_ptr<Vec2> v3;
    static const std::shared_ptr<Vec2> v4;
    static const std::shared_ptr<Vec2> v5;
    static const std::shared_ptr<Vec2> v6;

    static const std::shared_ptr<Vec3> v31;
    static const std::shared_ptr<Vec3> v32;
    static const std::shared_ptr<Vec3> v33;
    static const std::shared_ptr<Vec3> v34;

    static const std::shared_ptr<Rect> r1;
    static const std::shared_ptr<Rect> r2;
    static const std::shared_ptr<Rect> r3;

    static const std::shared_ptr<Circle> cr1;
    static const std::shared_ptr<Circle> cr2;
    static const std::shared_ptr<Circle> cr3;

    static const std::shared_ptr<Vec2> t1;

    static const std::shared_ptr<Color> c1;
    static const std::shared_ptr<Color> c2;
    static const std::shared_ptr<Color> c3;
    static const std::shared_ptr<Color> c4;

    static const std::shared_ptr<Point2> p1;
    static const std::shared_ptr<Point2> p2;
    static const std::shared_ptr<Point2> p3;

    static const std::shared_ptr<TextureRegion> tr1;
    static const std::shared_ptr<TextureRegion> tr2;

    static const std::shared_ptr<Mat> m1;
    static const std::shared_ptr<Mat> m2;
    static const std::shared_ptr<Mat> m3;

    static const std::shared_ptr<Bezier<Vec2>> bz2;
    static const std::shared_ptr<Bezier<Vec3>> bz3;
};

#endif //MINDUSTRY_SRC_ARC_UTIL_TMP_H
