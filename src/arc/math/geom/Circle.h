//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_CIRCLE_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_CIRCLE_H

#include <memory>

#include <arc/Utilities.h>
#include <arc/math/geom/Shape2D.h>

// Forward decl to avoid circular reference
class Vec2;

class Circle : public Shape2D, public std::enable_shared_from_this<Circle>, public Stringable {
public:
    float x{}, y{};
    float radius{};

    Circle();

    Circle(float x, float y, float radius);

    Circle(const std::shared_ptr<Vec2>& position, float radius);

    Circle(const Circle& circle);

    Circle(const std::shared_ptr<Circle>& circle);

    Circle(const std::shared_ptr<Vec2>& center, const std::shared_ptr<Vec2>& edge);

    std::shared_ptr<Circle> set(float x1, float y1, float radius1);

    std::shared_ptr<Circle> set(const std::shared_ptr<Vec2>& position, float radius1);

    std::shared_ptr<Circle> set(const std::shared_ptr<Circle>& circle);

    std::shared_ptr<Circle> set(const std::shared_ptr<Vec2>& center, const std::shared_ptr<Vec2>& edge);

    std::shared_ptr<Circle> setPosition(const std::shared_ptr<Vec2>& position);

    std::shared_ptr<Circle> setPosition(float x1, float y1);

    void setX(float x1);

    void setY(float y1);

    void setRadius(float radius1);

    bool contains(float x1, float y1) override;

    bool contains(std::shared_ptr<Vec2> point) override;

    bool contains(const std::shared_ptr<Circle>& c) const;

    bool overlaps(const std::shared_ptr<Circle>& c) const;

    std::string toString() override;

    float circumference() const;

    float area() const;

    bool operator==(std::shared_ptr<Circle>& o);
};

template<>
struct std::hash<Circle> {
    std::size_t operator()(const Circle& c) {
        std::size_t prime = 41;
        std::size_t result = 1;
        result = prime * result + std::bit_cast<std::uint32_t>(c.radius);
        result = prime * result + std::bit_cast<std::uint32_t>(c.x);
        result = prime * result + std::bit_cast<std::uint32_t>(c.y);
        return result;
    }
};

#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_CIRCLE_H
