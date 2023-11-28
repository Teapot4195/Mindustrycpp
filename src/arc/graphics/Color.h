//
// Created by teapot on 22/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_COLOR_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_COLOR_H

#include <memory>
#include <cstdint>
#include <vector>

// Forward decl, to avoid circular reference
class Vec3;

class Color {
public:
    static const std::shared_ptr<Color> white, lightGray, gray, darkGray, black, clear;
    static const float whiteFloatBits, clearFloatBits, blackFloatBits;
    static const int whiteRgba, clearRgba, blackRgba;
    static const std::shared_ptr<Color> blue, navy, royal, slate, sky, cyan, teal, green, acid, lime, forest, olive;
    static const std::shared_ptr<Color> yellow, gold, goldenrod, orange, brown, tan, brick, red, scarlet, crimson;
    static const std::shared_ptr<Color> coral, salmon, pink, magenta, purple, violet, maroon;

private:
    static float tmpHSV[3];

public:
    float r_{}, g_{}, b_{}, a_{};

    Color();

    Color(int rgba8888_);

    Color(float r, float g, float b, float a);

    Color(float r, float g , float b);

    Color(Color& col);

    static std::shared_ptr<Color> valueOf(std::string hex);

    static std::shared_ptr<Color> valueOf(const std::shared_ptr<Color>& color, std::string hex);

    static int parseHex(std::string string, int from, int to);

    static float toFloatBits(int r, int g, int b, int a);

    static float toFloatBits(float r, float g, float b, float a);

    static double toDoubleBits(float r, float g, float b, float a);

    static double toDoubleBits(int r, int g, int b, int a);

    std::shared_ptr<Color> fromDouble(double value);

    static int abgr(int r, int g, int b, int a);

    static int alpha(float alpha);

    static int luminanceAlpha(float luminance, float alpha);

    static int rgb565(float r, float g, float b);

    static int rgba4444(float r, float g, float b, float a);

    static int rgb888(float r, float g, float b);

    static int rgba8888(float r, float g, float b, float a);

    static int argb8888(float a, float r, float g, float b);

    static int packRgba(int r, int g, int b, int a);

    int rgba565() const;

    int rgba4444() const;

    int rgb888() const;

    int rgba8888() const;

    int argb8888() const;

    std::shared_ptr<Color> rgb565(int value);

    std::shared_ptr<Color> rgba4444(int value);

    std::shared_ptr<Color> rgb888(int value);

    std::shared_ptr<Color> rgba8888(int value);

    std::shared_ptr<Color> argb8888(int value);

    std::shared_ptr<Color> abgr8888(int value);

    static std::shared_ptr<Color> grays(float value);

    static std::shared_ptr<Color> rgb(int r, int g, int b);

    static std::uint32_t floatToIntColor(float value);

    static float intToFloatColor(std::uint32_t value);

    std::shared_ptr<Color> rand();

    std::shared_ptr<Color> randHue();

    float diff(const std::shared_ptr<Color>& other) const;

    int rgba() const;

    std::shared_ptr<Color> set(const std::shared_ptr<Color>& color);

    std::shared_ptr<Color> set(const std::shared_ptr<Vec3>& vec);

    std::shared_ptr<Color> mul(const std::shared_ptr<Color>& color);

    std::shared_ptr<Color> mul(float value);

    std::shared_ptr<Color> mula(float value);

    std::shared_ptr<Color> add(const std::shared_ptr<Color>& color);

    std::shared_ptr<Color> sub(const std::shared_ptr<Color>& color);

    std::shared_ptr<Color> clamp();

    std::shared_ptr<Color> set(float r1, float g1, float b1, float a1);

    std::shared_ptr<Color> set(float r1, float g1, float b1);

    std::shared_ptr<Color> set(int rgba);

    float sum() const;

    std::shared_ptr<Color> add(float r1, float g1, float b1, float a1);

    std::shared_ptr<Color> add(float r1, float g1, float b1);

    std::shared_ptr<Color> sub(float r1, float g1, float b1, float a1);

    std::shared_ptr<Color> sub(float r1, float g1, float b1);

    std::shared_ptr<Color> inv();

    std::shared_ptr<Color> r(float r1);

    std::shared_ptr<Color> g(float g1);

    std::shared_ptr<Color> b(float b1);

    std::shared_ptr<Color> a(float a1);

    std::shared_ptr<Color> mulA(float r1);

    std::shared_ptr<Color> mul(float r1, float g1, float b1, float a1);

    std::shared_ptr<Color> lerp(const std::shared_ptr<Color>& target, float t);

    std::shared_ptr<Color> premultiplyAlpha();

    std::shared_ptr<Color> write(const std::shared_ptr<Color>& to);

    float hue() const;

    float saturation() const;

    float value() const;

    std::shared_ptr<Color> hue(float amount);

    std::shared_ptr<Color> saturation(float amount);

    std::shared_ptr<Color> value(float amount);

    std::shared_ptr<Color> shiftHue(float amount);

    std::shared_ptr<Color> shiftSaturation(float amount);

    std::shared_ptr<Color> shiftValue(float amount);

    bool equals(const std::shared_ptr<Color>& o);

    float toFloatBits() const;

    double toDoubleBits() const;

    std::uint32_t abgr() const;

    std::string toString() const;

    void toString(std::string &builder) const;

    std::shared_ptr<Color> fromHsv(float h, float s, float v);

    std::shared_ptr<Color> fromHsv(float* hsv);

    float* toHsv(float* hsv) const;

    static std::shared_ptr<Color> HSVtoRGB(float h, float s, float v, float alpha);

    static std::shared_ptr<Color> HSVtoRGB(float h, float s, float v);

    static std::shared_ptr<Color> HSVtoRGB(float h, float s, float v, std::shared_ptr<Color> targetColor);

    static std::vector<int> RGBtoHSV(const std::shared_ptr<Color>& c);

    static std::vector<int> RGBtoHSV(float r, float g, float b);

    std::shared_ptr<Color> cpy();

    std::shared_ptr<Color> lerp(std::vector<std::shared_ptr<Color>> colors, float s);

    static int ri(float rgba);

    static int gi(float rgba);

    static int bi(float rgba);

    static int ai(float rgba);

    static int muli(int ca, int cb);

    static int muli(int rgba, float value);

private:
    static int clampf(float value);
};

template<>
struct std::hash<Color> {
    std::size_t operator()(const Color& o) {
        std::size_t result = (o.r_ != +0.0f ? std::bit_cast<std::uint32_t>(o.r_) : 0);
        result = 31 * result + (o.g_ != +0.0f ? std::bit_cast<std::uint32_t>(o.g_) : 0);
        result = 31 * result + (o.b_ != +0.0f ? std::bit_cast<std::uint32_t>(o.b_) : 0);
        result = 31 * result + (o.a_ != +0.0f ? std::bit_cast<std::uint32_t>(o.a_) : 0);

        return result;
    }
};

#endif //MINDUSTRY_SRC_ARC_GRAPHICS_COLOR_H
