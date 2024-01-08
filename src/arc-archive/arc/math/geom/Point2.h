//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_POINT2_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_POINT2_H

#include <memory>

#include <arc/Utilities.h>

class Point2 : public std::enable_shared_from_this<Point2>, public Stringable {
public:
    int x_{}, y_{};

    Point2();

    Point2(int x1, int y1);

    Point2(const std::shared_ptr<Point2>& point);

    static std::shared_ptr<Point2> unpack(int pos);

    static int pack(int x, int y);

    static short x(int pos);

    static short y(int pos);

    int pack() const;

    std::shared_ptr<Point2> set(const std::shared_ptr<Point2>& point);

    std::shared_ptr<Point2> set(int x, int y);

    float dst2(const std::shared_ptr<Point2>& other) const;

    float dst2(int x, int y) const;

    float dst(const std::shared_ptr<Point2>& other) const;

    float dst(int x, int y) const;

    std::shared_ptr<Point2> add(const std::shared_ptr<Point2>& other);

    std::shared_ptr<Point2> add(int x, int y);

    std::shared_ptr<Point2> sub(const std::shared_ptr<Point2>& other);

    std::shared_ptr<Point2> sub(int x, int y);

    std::shared_ptr<Point2> cpy();

    std::shared_ptr<Point2> rotate(int steps);

    bool equals(int x, int y) const;

    static bool equals(int x, int y, int ox, int oy);

    bool operator==(const std::shared_ptr<Point2>& g) const;

    std::string toString() override;
};


#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_POINT2_H
