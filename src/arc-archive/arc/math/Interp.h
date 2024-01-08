//
// Created by teapot on 23/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_MATH_INTERP_H
#define MINDUSTRY_SRC_ARC_MATH_INTERP_H

#include <cmath>
#include <vector>
#include <stdexcept>
#include <memory>
#include <functional>

#include "Mathf.h"

class   Interp {
protected:
    virtual ~Interp() = default;

public:
    virtual float apply(float a) = 0;

    virtual float apply(float start, float end, float a) {
        return start + (end - start) * apply(a);
    }

    float operator()(float a) {
        return apply(a);
    }

    float operator()(float start, float end, float a) {
        return apply(start, end, a);
    }

public:
    class Pow;
    class PowIn;
    class PowOut;
    class Exp;
    class ExpIn;
    class ExpOut;
    class Elastic;
    class ElasticIn;
    class ElasticOut;
    class Bounce;
    class BounceOut;
    class BounceIn;
    class Swing;
    class SwingOut;
    class SwingIn;

    static std::shared_ptr<Interp> linear, reverse, smooth, smooth2, one, zero, slope, smoother, fade, pow2, slowFast, pow2Out;
    static std::shared_ptr<Interp> fastSlow, pow2InInverse, pow2OutInverse, pow3, pow3In, pow3Out, pow3InInverse, pow3OutInverse;
    static std::shared_ptr<Interp> pow4, pow4In, pow4Out, pow5, pow5In, pow10In, pow10Out, pow5Out, sine, sineIn, sineOut;
    static std::shared_ptr<Interp> exp10, exp10In, exp10Out, exp5, exp5In, exp5Out, circle, circleIn, circleOut, elastic;
    static std::shared_ptr<Interp> elasticIn, elasticOut, swing, swingIn, sinwgOut, bounce, bounceIn, bounceOut, pow2In;
};

class IF : public Interp {
private:
    std::function<float(float)> apply_func;

public:
    IF(std::function<float(float)>&& func) : apply_func(std::move(func)) {}

    IF& operator=(const std::function<float(float)>&& func) {
        apply_func = func;
    }

    float apply(float a) override {
        return apply_func(a);
    }
};

class Interp::Pow : public Interp {
protected:
    const float power;
public:
    ~Pow() override = default;

    explicit Pow(float power) : power(power) {}

    explicit Pow(int power) : power((float)power) {}

    float apply(float a) override {
        if (a <= 0.5f)
            return (float)std::pow(a * 2, power) / 2;
        return (float)std::pow((a - 1) * 2, power) / (float)(std::fmod(power, 2) == 0 ? -2 : 2) + 1;
    }
};

class Interp::PowIn : public Pow {
public:
    explicit PowIn(float power) : Pow(power) {}

    explicit PowIn(int power) : Pow(power) {}

    float apply(float a) override {
        return (float)std::pow(a, power);
    }
};

class Interp::PowOut : public Pow {
public:
    explicit PowOut(int power) : Pow(power) {}

    explicit PowOut(float power) : Pow(power) {}

    float apply(float a) override {
        return (float)std::pow(a - 1, power) * (float)(std::fmod(power, 2) == 0 ? -1 : 1) + 1;
    }
};

class Interp::Exp : public Interp {
protected:
    const float value, power, min, scale;

public:
    Exp(float value, float power) : value(value), power(power), min((float)std::pow(value, -power)), scale(1 / (1 - min)) {}

    float apply(float a) override {
        if (a <= 0.5f)
            return ((float)std::pow(value, power * (a * 2 - 1)) - min) * scale / 2;
        return (2 - ((float)std::pow(value, -power * (a * 2 - 1)) - min) * scale) / 2;
    }
};

class Interp::ExpIn : public Exp {
public:
    ExpIn(float value, float power) : Exp(value, power) {}

    float apply(float a) override {
        return ((float)std::pow(value, power * (a - 1)) - min) * scale;
    }
};

class Interp::ExpOut : public Exp {
public:
    ExpOut(float value, float power) : Exp(value, power) {}

    float apply(float a) override {
        return 1 - ((float)std::pow(value, -power * a ) - min) * scale;
    }
};

class Interp::Elastic : public Interp {
protected:
    const float value, power, scale, bounces;

public:
    Elastic(float value, float power, int bounces, float scale) :
        value(value),
        power(power),
        scale(scale),
        bounces((float)bounces * Mathf::PI * (float)(bounces % 2 == 0 ? 1 : -1)) {}

    float apply(float a) override {
        if (a <= 0.5f) {
            a *= 2;
            return (float)std::pow(value, power * (a - 1)) * Mathf::sin(a * bounces) * scale / 2;
        }
        a = 1 - a;
        a *= 2;
        return 1 - (float)std::pow(value, power * (a - 1)) * Mathf::sin((a) * bounces) * scale / 2;
    }
};

class Interp::ElasticIn : public Elastic {
public:
    ElasticIn(float value, float power, int bounces, float scale) : Elastic(value, power, bounces, scale) {}

    float apply(float a) override {
        if (a >= 0.99f)
            return 1;
        return (float)std::pow(value, power * (a - 1)) * Mathf::sin(a * bounces) * scale;
    }
};

class Interp::ElasticOut : public Elastic {
public:
    ElasticOut(float value, float power, int bounces, float scale) : Elastic(value, power, bounces, scale) {}

    float apply(float a) override {
        if (a == 0)
            return 0;
        a = 1 - a;
        return (1 - (float)std::pow(value, power * (a - 1)) * Mathf::sin(a * bounces) * scale);
    }
};

class Interp::BounceOut : public Interp {
protected:
    const std::vector<float> widths, heights;

private:
    static std::vector<float> getWidths(int bounces) {
        if (bounces < 2 || bounces > 5)
            throw std::invalid_argument("bounces cannot be < 2 or > 5: " + std::to_string(bounces));
        std::vector<float> ret(bounces);
        switch(bounces) {
            case 2:
                ret[0] = 0.6f;
                ret[1] = 0.4f;
                break;
            case 3:
                ret[0] = ret[1] = 0.4f;
                ret[2] = 0.2f;
                break;
            case 4:
                ret[0] = ret[1] = 0.34f;
                ret[2] = 0.2f;
                ret[3] = 0.15f;
                break;
            case 5:
                ret[0] = ret[1] = 0.3f;
                ret[2] = 0.2f;
                ret[3] = ret[4] = 0.1f;
                break;
        }
        ret[0] *= 2;
        return ret;
    }

    static std::vector<float> getHeights(int bounces) {
        // C++ cannot guarantee the order that the const variables are initialized in an initializer list
        // We must check for bounces within bounds in both init helper functions
        if (bounces < 2 || bounces > 5)
            throw std::invalid_argument("bounces cannot be < 2 or > 5: " + std::to_string(bounces));
        std::vector<float> ret(bounces);
        ret[0] = 1;
        switch(bounces) {
            case 2:
                ret[1] = 0.33f;
                break;
            case 3:
                ret[1] = 0.33f;
                ret[2] = 0.1f;
                break;
            case 4:
                ret[1] = 0.26f;
                ret[2] = 0.11f;
                ret[3] = 0.03f;
                break;
            case 5:
                ret[1] = 0.45f;
                ret[2] = 0.3f;
                ret[3] = 0.15f;
                ret[3] = 0.06f;
                break;
        }
        return ret;
    }

public:
    BounceOut(const std::vector<float>& widths, const std::vector<float>& heights) : widths(widths), heights(heights) {
        if (widths.size() != heights.size())
            throw std::invalid_argument("Must be same number of widths and heights.");
    }

    explicit BounceOut(int bounces) : widths(getWidths(bounces)), heights(getHeights(bounces)) {}

    float apply(float a) override {
        if (a == 1)
            return 1;
        a += widths[0] / 2;
        float width = 0, height = 0;
        for (std::size_t i = 0, n = widths.size(); i < n; i++) {
            width = widths[i];
            if (a <= width) {
                height = heights[i];
                break;
            }
            a -= width;
        }
        a /= width;
        float z = 4 / width * height * a;
        return 1 - (z - z * a) * width;
    }
};

class Interp::Bounce : public BounceOut {
public:
    Bounce(const std::vector<float>& widths, const std::vector<float>& heights) : BounceOut(widths, heights) {}

    explicit Bounce(int bounces) : BounceOut(bounces) {}

private:
    float out(float a) {
        float test = a + widths[0] / 2;
        if (test < widths[0])
            return test / (widths[0] / 2) - 1;
        return BounceOut::apply(a);
    }

public:
    float apply(float a) override {
        if (a <= 0.5f)
            return (1 - out(1 - a * 2)) / 2;
        return out(a * 2 - 1) / 2 + 0.5f;
    }
};

class Interp::BounceIn : public BounceOut {
public:
    BounceIn(const std::vector<float>& widths, const std::vector<float>& heights) : BounceOut(widths, heights) {}

    explicit BounceIn(int bounces) : BounceOut(bounces) {}

public:
    float apply(float a) override {
        return 1 - BounceOut::apply(1 - a);
    }
};

class Interp::Swing : public Interp {
private:
    const float scale;

public:
    explicit Swing(float scale) : scale(scale * 2) {}

    float apply(float a) override {
        if (a <= 0.5f) {
            a *= 2;
            return a * a * ((scale + 1) * a - scale) / 2;
        }
        a--;
        a *= 2;
        return a * a * ((scale + 1) * a + scale) / 2 + 1;
    }
};

class Interp::SwingOut : public Interp {
private:
    const float scale;

public:
    explicit SwingOut(float scale) : scale(scale) {}

    float apply(float a) override {
        a--;
        return a * a * ((scale + 1) * a + scale) + 1;
    }
};

class Interp::SwingIn : public Interp {
private:
    const float scale;

public:
    explicit SwingIn(float scale) : scale(scale) {}

    float apply(float a) override {
        return a * a * ((scale + 1) * a - scale);
    }
};

#define ms(lambda) std::make_shared<IF>([](float a) -> float lambda)

std::shared_ptr<Interp> Interp::linear = ms({
    return a;
});
std::shared_ptr<Interp> Interp::reverse = ms({
    return 1.f - a;
});
std::shared_ptr<Interp> Interp::smooth = ms({
    return a * a * (3 - 2 * a);
});
std::shared_ptr<Interp> Interp::smooth2 = ms({
    a = a * a * (3 - 2 * a);
    return a * a * (3 - 2 * a);
});
std::shared_ptr<Interp> Interp::one = ms({
    return 1.f;
});
std::shared_ptr<Interp> Interp::zero = ms({
    return 0.f;
});
std::shared_ptr<Interp> Interp::slope = ms({
    return Mathf::slope(a);
});
std::shared_ptr<Interp> Interp::smoother = ms({
    return a * a * a * (a * (a * 6 - 15) + 10);
});
std::shared_ptr<Interp> Interp::fade = smoother;
std::shared_ptr<Interp> Interp::pow2 = std::make_shared<Pow>(2);
std::shared_ptr<Interp> Interp::slowFast = pow2In;
std::shared_ptr<Interp> Interp::pow2In = std::make_shared<PowIn>(2);
std::shared_ptr<Interp> Interp::pow2Out = std::make_shared<PowOut>(2);
std::shared_ptr<Interp> Interp::fastSlow = pow2Out;
std::shared_ptr<Interp> Interp::pow2InInverse = ms({
    return (float)std::sqrt(a);
});
std::shared_ptr<Interp> Interp::pow2OutInverse = ms({
    return 1 - (float)std::sqrt(-(a - 1));
});
std::shared_ptr<Interp> Interp::pow3 = std::make_shared<Pow>(3);
std::shared_ptr<Interp> Interp::pow3In = std::make_shared<PowIn>(3);
std::shared_ptr<Interp> Interp::pow3Out = std::make_shared<PowOut>(3);
std::shared_ptr<Interp> Interp::pow3InInverse = ms({
    return (float)std::cbrt(a);
});
std::shared_ptr<Interp> Interp::pow3OutInverse = ms({
    return 1 - (float)std::cbrt(-(a - 1));
});
std::shared_ptr<Interp> Interp::pow4 = std::make_shared<Pow>(4);
std::shared_ptr<Interp> Interp::pow4In = std::make_shared<PowIn>(4);
std::shared_ptr<Interp> Interp::pow4Out = std::make_shared<PowOut>(4);
std::shared_ptr<Interp> Interp::pow5 = std::make_shared<Pow>(5);
std::shared_ptr<Interp> Interp::pow5In = std::make_shared<PowIn>(5);
std::shared_ptr<Interp> Interp::pow10In = std::make_shared<PowIn>(10);
std::shared_ptr<Interp> Interp::pow10Out = std::make_shared<PowOut>(10);
std::shared_ptr<Interp> Interp::pow5Out = std::make_shared<PowOut>(5);
std::shared_ptr<Interp> Interp::sine = ms({
    return (1 - Mathf::cos(a * Mathf::PI)) / 2;
});
std::shared_ptr<Interp> Interp::sineIn = ms({
    return 1 - Mathf::cos(a * Mathf::PI / 2);
});
std::shared_ptr<Interp> Interp::sineOut = ms({
    return Mathf::sin(a * Mathf::PI / 2);
});
std::shared_ptr<Interp> Interp::exp10 = std::make_shared<Exp>(2, 10);
std::shared_ptr<Interp> Interp::exp10In = std::make_shared<ExpIn>(2, 10);
std::shared_ptr<Interp> Interp::exp10Out = std::make_shared<ExpOut>(2, 10);
std::shared_ptr<Interp> Interp::exp5 = std::make_shared<Exp>(2, 5);
std::shared_ptr<Interp> Interp::exp5In = std::make_shared<ExpIn>(2, 5);
std::shared_ptr<Interp> Interp::exp5Out = std::make_shared<ExpOut>(2, 5);
std::shared_ptr<Interp> Interp::circle = ms({
    if (a <= 0.5f) {
        a *= 2;
        return (1 - (float)std::sqrt(1 - a * a)) / 2;
    }
    a--;
    a *= 2;
    return ((float)std::sqrt(1 - a * a) + 1) / 2;
});
std::shared_ptr<Interp> Interp::circleIn = ms({
    return 1 - (float)std::sqrt(1 - a * a);
});
std::shared_ptr<Interp> Interp::circleOut = ms({
    a--;
    return (float)std::sqrt(1 - a * a);
});
std::shared_ptr<Interp> Interp::elastic = std::make_shared<Elastic>(2, 10, 7, 1);
std::shared_ptr<Interp> Interp::elasticIn = std::make_shared<ElasticIn>(2, 10, 6, 1);
std::shared_ptr<Interp> Interp::elasticOut = std::make_shared<ElasticOut>(2, 10, 7, 1);
std::shared_ptr<Interp> Interp::swing = std::make_shared<Swing>(1.5f);
std::shared_ptr<Interp> Interp::swingIn = std::make_shared<SwingIn>(2.f);
std::shared_ptr<Interp> Interp::sinwgOut = std::make_shared<SwingOut>(2.f);
std::shared_ptr<Interp> Interp::bounce = std::make_shared<Bounce>(4);
std::shared_ptr<Interp> Interp::bounceIn = std::make_shared<BounceIn>(4);
std::shared_ptr<Interp> Interp::bounceOut = std::make_shared<BounceOut>(4);

#undef ms

#endif //MINDUSTRY_SRC_ARC_MATH_INTERP_H
