//
// Created by teapot on 09/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_GEOM_BEZIER_H
#define MINDUSTRY_SRC_ARC_MATH_GEOM_BEZIER_H

#include <cmath>
#include <concepts>
#include <memory>

#include <arc/struct.h>
#include <arc/math/Mathf.h>
#include <arc/math/geom/Path.h>
#include <arc/math/geom/Vector.h>
#include <arc/util/Exception.h>

template<typename T> requires std::derived_from<T, Vector<T>>
class Bezier : public Path<T> {
public:
    Seq<std::shared_ptr<T>> points = Seq<std::shared_ptr<T>>();

private:
    std::shared_ptr<T> tmp, tmp2, tmp3;

public:
    Bezier() = default;

    Bezier(std::initializer_list<std::shared_ptr<T>> points);

    Bezier(std::vector<std::shared_ptr<T>> points, int offset, int length);

//    Bezier(Seq<std::shared_ptr<T>> points, int offset, int length);

    static std::shared_ptr<T>
    linear(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1, std::shared_ptr<T> tmp);

    static std::shared_ptr<T>
    linearDerivative(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                     std::shared_ptr<T> tmp);

    static std::shared_ptr<T>
    quadratic(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1, std::shared_ptr<T> p2,
              std::shared_ptr<T> tmp);

    static std::shared_ptr<T>
    quadraticDerivative(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                        std::shared_ptr<T> p2, std::shared_ptr<T> tmp);

    static std::shared_ptr<T>
    cubic(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1, std::shared_ptr<T> p2,
          std::shared_ptr<T> p3, std::shared_ptr<T> tmp);

    static std::shared_ptr<T>
    cubicDerivative(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                    std::shared_ptr<T> p2, std::shared_ptr<T> p3, std::shared_ptr<T> tmp);

    std::shared_ptr<Bezier<T>> set(std::initializer_list<std::shared_ptr<T>> points);

    std::shared_ptr<Bezier<T>> set(std::vector<std::shared_ptr<T>> p, int offset, int length);

    std::shared_ptr<T> valueAt(std::shared_ptr<T> out, float t) override;

    std::shared_ptr<T> derivativeAt(std::shared_ptr<T> out, float t) override;

    float approximate(std::shared_ptr<T> v) override;

    float locate(std::shared_ptr<T> v) override;

    float approxLength(int samples) override;
};

template<typename T>
requires std::derived_from<T, Vector<T>>float Bezier<T>::approxLength(int samples) {
    float tempLength = 0;
    for (int i = 0; i < samples; ++i) {
        tmp2->set(tmp3);
        valueAt(tmp3, (float)(i) / ((float) samples - 1));
        if (i > 0)
            tempLength += tmp2->dst(tmp3);
    }
    return tempLength;
}

template<typename T>
requires std::derived_from<T, Vector<T>>float Bezier<T>::locate(std::shared_ptr<T> v) {
    return approximate(v);
}

template<typename T>
requires std::derived_from<T, Vector<T>>float Bezier<T>::approximate(std::shared_ptr<T> v) {
    std::shared_ptr<T> p1 = points[0];
    std::shared_ptr<T> p2 = points[points.size() - 1];
    float l1Sqr = p1->dst2(p2);
    float l2Sqr = v->dst2(p2);
    float l3Sqr = v->dst2(p1);
    float l1 = std::sqrt(l1Sqr);
    float s = (l2Sqr + l1Sqr - l3Sqr) / (2 * l1);
    return Mathf::clamp((l1 - s) / l1, 0.f, 1.f);
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T> Bezier<T>::derivativeAt(std::shared_ptr<T> out, float t) {
    int n = points.size();
    if (n == 2)
        linearDerivative(out, t, points[0], points[1], tmp);
    else if (n == 3)
        quadraticDerivative(out, t, points[0], points[1], points[2], tmp);
    else if (n == 4)
        cubicDerivative(out, t, points[0], points[1], points[2], points[3], tmp);
    return out;
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T> Bezier<T>::valueAt(std::shared_ptr<T> out, float t) {
    int n = points.size();
    if (n == 2)
        linear(out, t, points[0], points[1], tmp);
    else if (n == 3)
        quadratic(out, t, points[0], points[1], points[2], tmp);
    else if (n == 4)
        cubic(out, t, points[0], points[1], points[2], points[3], tmp);
    return out;
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<Bezier<T>>
Bezier<T>::set(std::vector<std::shared_ptr<T>> p, int offset, int length) {
    if (length < 2 || length > 4)
        throw ArcRuntimeException("Only first, second and third degree Bezier curves are supported.");
    if (tmp)
        tmp = p[0].cpy();
    if (tmp2)
        tmp = p[0].cpy();
    if (tmp3)
        tmp = p[0].cpy();
    points.clear();
    points.insert(p.begin() + offset, p.begin() + offset + length);
    return std::make_shared<T>(this);
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<Bezier<T>>
Bezier<T>::set(std::initializer_list<std::shared_ptr<T>> p) {
    if (p.size() < 2 || p.size() > 4)
        throw ArcRuntimeException("Only first, second and third degree Bezier curves are supported.");
    if (tmp)
        tmp = p[0].cpy();
    if (tmp2)
        tmp = p[0].cpy();
    if (tmp3)
        tmp = p[0].cpy();
    points.clear();
    points.insert(p.begin(), p.begin() + p.size());
    return std::make_shared<T>(this);
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T>
Bezier<T>::cubicDerivative(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                           std::shared_ptr<T> p2, std::shared_ptr<T> p3, std::shared_ptr<T> tmp) {
    float dt = 1.f - t;
    float dt2 = dt * dt;
    float t2 = t * t;
    return out->set(p1)->sub(p0)->scl(dt2 * 3)->add(tmp->set(p2)->sub(p1)->scl(dt * t * 6))->add(
            tmp->set(p3)->sub(p2)->scl(t2 * 3));
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T>
Bezier<T>::cubic(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1, std::shared_ptr<T> p2,
                 std::shared_ptr<T> p3, std::shared_ptr<T> tmp) {
    float dt = 1.f - t;
    float dt2 = dt * dt;
    float t2 = t * t;
    return out->set(p0)->scl(dt2 * dt)->add(
            tmp->set(p1)->scl(3 * dt2 * t)->add(tmp->set(p2)->scl(3 * dt * t2))->add(tmp->set(p3)->scl(t2 * t)));
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T>
Bezier<T>::quadraticDerivative(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                               std::shared_ptr<T> p2, std::shared_ptr<T> tmp) {
    return out->set(p1)->sub(p0)->scl(2)->scl(1 - t)->add(tmp->set(p2)->sub(p1)->scl(t)->scl(2));
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T>
Bezier<T>::quadratic(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                     std::shared_ptr<T> p2, std::shared_ptr<T> tmp) {
    float dt = 1.f - t;
    return out->set(p0)->scl(dt * dt)->add(tmp->set(p1)->scl(2 * dt * t))->add(tmp->set(p2)->scl(t * t));
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T>
Bezier<T>::linearDerivative(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                            std::shared_ptr<T> tmp) {
    return out->set(p1)->sub(p0);
}

template<typename T>
requires std::derived_from<T, Vector<T>>std::shared_ptr<T>
Bezier<T>::linear(std::shared_ptr<T> out, float t, std::shared_ptr<T> p0, std::shared_ptr<T> p1,
                  std::shared_ptr<T> tmp) {
    return out->set(p0)->scl(1.f - t)->add(tmp->set(p1)->scl(t));
}

//template<typename T>
//requires std::derived_from<T, Vector<T>>
//Bezier<T>::Bezier(const Seq<std::shared_ptr<T>> points, int offset, int length) {
//    set(points, offset, length);
//}

template<typename T>
requires std::derived_from<T, Vector<T>>
Bezier<T>::Bezier(const std::vector<std::shared_ptr<T>> points, int offset, int length) {
    set(points, offset, length);
}

template<typename T>
requires std::derived_from<T, Vector<T>> Bezier<T>::Bezier(const std::initializer_list<std::shared_ptr<T>> points) {
    set(points);
}


#endif //MINDUSTRY_SRC_ARC_MATH_GEOM_BEZIER_H
