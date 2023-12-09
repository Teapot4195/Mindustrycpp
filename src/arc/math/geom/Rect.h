//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_RECT_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_RECT_H

#include <memory>
#include <vector>

#include <arc/Utilities.h>
#include <arc/math/geom/Shape2D.h>

// Forward decl to avoid circular reference
class Circle;

class Rect : public Shape2D, public std::enable_shared_from_this<Rect>, public Stringable {
public:
    static std::shared_ptr<Rect> tmp;
    static std::shared_ptr<Rect> tmp2;

    float x{}, y{}, width{}, height{};

    Rect();

    Rect(float x, float y, float width, float height);

    Rect(const Rect& rect);

    explicit Rect(const std::shared_ptr<Rect>& rect);

    std::shared_ptr<Rect> setCentered(float x1, float y1, float size);

    std::shared_ptr<Rect> setCentered(float x1, float y1, float width1, float height1);

    std::shared_ptr<Rect> set(float x1, float y1, float width1, float height1);

    float getX() const;

    std::shared_ptr<Rect> setX(float x1);

    float getY() const;

    std::shared_ptr<Rect> setY(float y1);

    float getWidth() const;

    std::shared_ptr<Rect> setWidth(float width);

    float getHeight() const;

    std::shared_ptr<Rect> setHeight(float height1);

    std::shared_ptr<Vec2> getPosition(const std::shared_ptr<Vec2>& position) const;

    std::shared_ptr<Rect> setPosition(const std::shared_ptr<Vec2>& position);

    std::shared_ptr<Rect> setPosition(float x1, float y1);

    std::shared_ptr<Rect> move(float cx, float cy);

    std::shared_ptr<Rect> setSize(float width1, float height1);

    std::shared_ptr<Rect> setSize(float sizeXY);

    std::shared_ptr<Vec2> getSize(const std::shared_ptr<Vec2>& size) const;

    static bool contains(float x, float y, float width, float height, float px, float py);

    bool contains(float x1, float y1) override;

    bool contains(const std::shared_ptr<Vec2>& point) override;

    bool contains(const std::shared_ptr<Circle>& circle) const;

    bool contains(const std::shared_ptr<Rect>& rect) const;

    bool overlaps(const std::shared_ptr<Rect>& r) const;

    bool overlaps(float rx, float ry, float rwidth, float rheight) const;

    std::shared_ptr<Rect> set(const std::shared_ptr<Rect>& rect);

    std::shared_ptr<Rect> grow(float amount);

    std::shared_ptr<Rect> grow(float amountX, float amountY);

    std::shared_ptr<Rect> merge(const std::shared_ptr<Rect>& rect);

    std::shared_ptr<Rect> normalize();

    std::shared_ptr<Rect> merge(float x1, float y1);

    std::shared_ptr<Rect> merge(const std::shared_ptr<Vec2>& vec);

    std::shared_ptr<Rect> merge(const std::vector<std::shared_ptr<Vec2>>& vecs);

    float getAspectRatio() const;

    std::shared_ptr<Vec2> getCenter(const std::shared_ptr<Vec2>& vector) const;

    std::shared_ptr<Rect> setCenter(float x1, float y1);

    std::shared_ptr<Rect> setCenter(const std::shared_ptr<Vec2>& position);

    std::shared_ptr<Rect> fitOutside(std::shared_ptr<Rect>& rect);

    std::shared_ptr<Rect> fitInside(std::shared_ptr<Rect>& rect);

    std::string toString() override;

    std::shared_ptr<Rect> fromString(std::string v);

    float area() const;

    float perimeter() const;

    bool operator==(const std::shared_ptr<Rect>& other);
};

template<>
struct std::hash<Rect> {
    std::size_t operator()(const Rect& o) {
        std::size_t prime = 31;
        std::size_t result = 1;
        result = prime * result + std::bit_cast<std::uint32_t>(o.height);
        result = prime * result + std::bit_cast<std::uint32_t>(o.width);
        result = prime * result + std::bit_cast<std::uint32_t>(o.x);
        result = prime * result + std::bit_cast<std::uint32_t>(o.y);
        return result;
    }
};

#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_RECT_H
