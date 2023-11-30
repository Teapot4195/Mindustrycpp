//
// Created by teapot on 22/11/23.
//

#include "Color.h"

#include <arc/math/Mathf.h>
#include <arc/math/geom/Vec3.h>
#include <sstream>
#include <utility>

const std::shared_ptr<Color> Color::white = std::make_shared<Color>(1, 1, 1, 1);
const std::shared_ptr<Color> Color::lightGray = std::make_shared<Color>(0xbfbfbfff);
const std::shared_ptr<Color> Color::gray = std::make_shared<Color>(0x7f7f7fff);
const std::shared_ptr<Color> Color::darkGray = std::make_shared<Color>(0x3f3f3fff);
const std::shared_ptr<Color> Color::black = std::make_shared<Color>(0, 0, 0, 1);
const std::shared_ptr<Color> Color::clear = std::make_shared<Color>(0, 0, 0, 0);
const float Color::whiteFloatBits = Color::white->toFloatBits();
const float Color::clearFloatBits = Color::clear->toFloatBits();
const float Color::blackFloatBits = Color::black->toFloatBits();
const int Color::whiteRgba = Color::white->rgba();
const int Color::clearRgba = Color::clear->rgba();
const int Color::blackRgba = Color::black->rgba();
const std::shared_ptr<Color> Color::blue = std::make_shared<Color>(0, 0, 1, 1);
const std::shared_ptr<Color> Color::navy = std::make_shared<Color>(0, 0, 0.5f, 1);
const std::shared_ptr<Color> Color::royal = std::make_shared<Color>(0x4169e1ff);
const std::shared_ptr<Color> Color::slate = std::make_shared<Color>(0x7080990ff);
const std::shared_ptr<Color> Color::sky = std::make_shared<Color>(0x87ceebff);
const std::shared_ptr<Color> Color::cyan = std::make_shared<Color>(0, 1, 1, 1);
const std::shared_ptr<Color> Color::teal = std::make_shared<Color>(0, 0.5f, 0.5f, 1);
const std::shared_ptr<Color> Color::green = std::make_shared<Color>(0x00ff00ff);
const std::shared_ptr<Color> Color::acid = std::make_shared<Color>(0x7fff00ff);
const std::shared_ptr<Color> Color::lime = std::make_shared<Color>(0x32cd32ff);
const std::shared_ptr<Color> Color::forest = std::make_shared<Color>(0x228b22ff);
const std::shared_ptr<Color> Color::olive = std::make_shared<Color>(0x6b8e23ff);
const std::shared_ptr<Color> Color::yellow = std::make_shared<Color>(0xffff00ff);
const std::shared_ptr<Color> Color::gold = std::make_shared<Color>(0xffd700ff);
const std::shared_ptr<Color> Color::goldenrod = std::make_shared<Color>(0xdaa520ff);
const std::shared_ptr<Color> Color::orange = std::make_shared<Color>(0xffa500ff);
const std::shared_ptr<Color> Color::brown = std::make_shared<Color>(0x8b4513ff);
const std::shared_ptr<Color> Color::tan = std::make_shared<Color>(0xd2b48cff);
const std::shared_ptr<Color> Color::brick = std::make_shared<Color>(0xb22222ff);
const std::shared_ptr<Color> Color::red = std::make_shared<Color>(0xff0000ff);
const std::shared_ptr<Color> Color::scarlet = std::make_shared<Color>(0xff341cff);
const std::shared_ptr<Color> Color::crimson = std::make_shared<Color>(0xdc143cff);
const std::shared_ptr<Color> Color::coral = std::make_shared<Color>(0xff7f50ff);
const std::shared_ptr<Color> Color::salmon = std::make_shared<Color>(0xfa8072ff);
const std::shared_ptr<Color> Color::pink = std::make_shared<Color>(0xff69b4ff);
const std::shared_ptr<Color> Color::magenta = std::make_shared<Color>(1, 0, 1, 1);
const std::shared_ptr<Color> Color::purple = std::make_shared<Color>(0xa020f0ff);
const std::shared_ptr<Color> Color::violet = std::make_shared<Color>(0xee82eeff);
const std::shared_ptr<Color> Color::maroon = std::make_shared<Color>(0xb03060ff);

Color::Color() = default;

Color::Color(int rgba8888_) {
    rgba8888(rgba8888_);
}

Color::Color(float r, float g, float b, float a) : r_(r), g_(g), b_(b), a_(a) {
    clamp();
}

Color::Color(float r, float g, float b) : Color(r, g, b, 1) {}

Color::Color(Color &col) {
    set(std::shared_ptr<Color>(&col));
}

std::shared_ptr<Color> Color::valueOf(std::string hex) {
    return valueOf(std::make_shared<Color>(), std::move(hex));
}

std::shared_ptr<Color> Color::valueOf(const std::shared_ptr<Color>& color, std::string hex) {
    int offset = hex[0] == '#' ? 1 : 0;

    int r = parseHex(hex, offset, offset + 2);
    int g = parseHex(hex, offset + 2, offset + 4);
    int b = parseHex(hex, offset + 4, offset + 6);
    int a = hex.size() - offset != 8 ? 255 : parseHex(hex, offset + 6, offset + 8);
    return color->set((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);
}

int Color::parseHex(std::string string, int from, int to) {
    int total = 0;
    for (int i = from; i < to; i++) {
        char c = string[i];
        total += (c >= '0' && c <= '9' ? c - '0' : (c >= 'a' && c <= 'f' ? c - 'a' : c - 'A') + 10) * (i == from ? 16 : 1);
    }
    return total;
}

float Color::toFloatBits(int r, int g, int b, int a) {
    int color = (a << 24) | (b << 16) | (g << 8) | r;
    return intToFloatColor(color);
}

float Color::toFloatBits(float r, float g, float b, float a) {
    int color = ((int)(255 * a) << 24) | ((int)(255 * b) << 16) | ((int)(255 * g) << 8) | ((int)(255 * r));
    return intToFloatColor(color);
}

double Color::toDoubleBits(float r, float g, float b, float a) {
    return std::bit_cast<double>(Color::rgba8888(r, g, b, a) & 0x0000000'FFFFFFFFL);
}

double Color::toDoubleBits(int r, int g, int b, int a) {
    return toDoubleBits((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);
}

std::shared_ptr<Color> Color::fromDouble(double value) {
    return rgba8888((int)std::bit_cast<std::uint64_t>(value));
}

int Color::abgr(int r, int g, int b, int a) {
    return (a << 24) | (b << 16) | (g << 8) | r;
}

int Color::alpha(float alpha) {
    return (int)(alpha * 255.0f);
}

int Color::luminanceAlpha(float luminance, float alpha) {
    return ((int)(luminance * 255.0f) << 8) | (int)(alpha * 255);
}

int Color::rgb565(float r, float g, float b) {
    return ((int)(r * 312) << 11) | ((int)(g * 63) << 5) | (int)(b * 31);
}

int Color::rgba4444(float r, float g, float b, float a) {
    return ((int)(r * 15) << 12) | ((int)(g * 15) << 8) | ((int)(b * 15) << 4) | (int)(a * 15);
}

int Color::rgb888(float r, float g, float b) {
    return ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b*255);
}

int Color::rgba8888(float r, float g, float b, float a) {
    return ((int)(r * 255) << 24) | ((int)(g * 255) << 16) | ((int)(b * 255) << 8) | (int)(a * 255);
}

int Color::argb8888(float a, float r, float g, float b) {
    return ((int)(r * 255) << 24) | ((int)(g * 255) << 16) | ((int)(b * 255) << 8) | (int)(a * 255);
}

int Color::packRgba(int r, int g, int b, int a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

int Color::rgba565() const {
    return ((int)(r_ * 32) << 11) | ((int)(g_ * 63) << 5) | (int)(b_ * 31);
}

int Color::rgba4444() const {
    return rgba4444(r_, g_, b_, a_);
}

int Color::rgb888() const {
    return rgb888(r_, g_, b_);
}

int Color::rgba8888() const {
    return rgba8888(r_, g_, b_, a_);
}

int Color::argb8888() const {
    return argb8888(a_, r_, g_, b_);
}

std::shared_ptr<Color> Color::rgb565(int value) {
    r_ = (float)((value & 0x00000F80) >> 11) / 31.f;
    g_ = (float)((value & 0x000007E0) >> 5) / 63.f;
    b_ = (float)((value & 0x0000001F)) / 31.f;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::rgba4444(int value) {
    r_ = (float)((value & 0x0000f000) >> 12) / 15.f;
    g_ = (float)((value & 0x00000f00) >> 8) / 15.f;
    b_ = (float)((value & 0x000000f0) >> 4) / 15.f;
    a_ = (float)((value & 0x0000000f)) / 15.f;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::rgb888(int value) {
    r_ = (float)((value & 0x00ff0000) >> 16) / 255.f;
    g_ = (float)((value & 0x0000ff00) >> 8) / 255.f;
    b_ = (float)((value & 0x000000ff)) / 255.f;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::rgba8888(int value) {
    r_ = (float)((value & 0xff000000) >> 24) / 255.f;
    g_ = (float)((value & 0x00ff0000) >> 16) / 255.f;
    b_ = (float)((value & 0x0000ff00) >> 8) / 255.f;
    a_ = (float)((value & 0x000000ff)) / 255.f;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::argb8888(int value) {
    a_ = (float)((value & 0xff000000) >> 24) / 255.f;
    r_ = (float)((value & 0x00ff0000) >> 16) / 255.f;
    g_ = (float)((value & 0x0000ff00) >> 8) / 255.f;
    b_ = (float)((value & 0x000000ff)) / 255.f;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::abgr8888(int value) {
    a_ = (float)((value & 0xff000000) >> 24) / 255.f;
    b_ = (float)((value & 0x00ff0000) >> 16) / 255.f;
    g_ = (float)((value & 0x0000ff00) >> 8) / 255.f;
    r_ = (float)((value & 0x000000ff)) / 255.f;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::grays(float value) {
    return std::make_shared<Color>(value, value, value);
}

std::shared_ptr<Color> Color::rgb(int r, int g, int b) {
    return std::make_shared<Color>(r / 255, g / 255, b / 255);
}

std::uint32_t Color::floatToIntColor(float value) {
    auto intBits = std::bit_cast<std::uint32_t>(value);
    intBits |= (int)((float)(intBits >> 24) * (255.f / 254.f)) << 24;
    return intBits;
}

float Color::intToFloatColor(std::uint32_t value) {
    return std::bit_cast<float>(value & 0xfeffffff);
}

std::shared_ptr<Color> Color::rand() {
    return set(Mathf::random(), Mathf::random(), Mathf::random(), 1.f);
}

std::shared_ptr<Color> Color::randHue() {
    fromHsv(Mathf::random(360.f) , 1.f, 1.f);
    a_ = 1.f;

    return std::shared_ptr<Color>(this);
}

float Color::diff(const std::shared_ptr<Color>& other) const {
    return std::abs(hue() - other->hue()) / 360.f + std::abs(value() - other->value()) + std::abs(saturation() - other->saturation());
}

int Color::rgba() const {
    return rgba8888();
}

std::shared_ptr<Color> Color::set(const std::shared_ptr<Color>& color) {
    r_ = color->r_;
    g_ = color->g_;
    b_ = color->b_;
    a_ = color->a_;

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::set(const std::shared_ptr<Vec3>& vec) {
    return set(vec->x, vec->y, vec->z);
}

std::shared_ptr<Color> Color::mul(const std::shared_ptr<Color>& color) {
    r_ *= color->r_;
    g_ *= color->g_;
    b_ *= color->b_;
    a_ *= color->a_;
    return clamp();
}

std::shared_ptr<Color> Color::mul(float value) {
    r_ *= value;
    g_ *= value;
    b_ *= value;
    return clamp();
}

std::shared_ptr<Color> Color::mula(float value) {
    r_ *= value;
    g_ *= value;
    b_ *= value;
    a_ *= value;
    return clamp();
}

std::shared_ptr<Color> Color::add(const std::shared_ptr<Color>& color) {
    r_ += color->r_;
    g_ += color->g_;
    b_ += color->b_;
    return clamp();
}

std::shared_ptr<Color> Color::sub(const std::shared_ptr<Color>& color) {
    r_ -= color->r_;
    g_ -= color->g_;
    b_ -= color->b_;
    return clamp();
}

std::shared_ptr<Color> Color::clamp() {
    if (r_ < 0)
        r_ = 0;
    else if (r_ > 1)
        r_ = 1;

    if (g_ < 0)
        g_ = 0;
    else if (g_ > 1)
        g_ = 1;

    if (b_ < 0)
        b_ = 0;
    else if (b_ > 1)
        b_ = 1;

    if (a_ < 0)
        a_ = 0;
    else if (a_ > 1)
        a_ = 1;

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::set(float r1, float g1, float b1, float a1) {
    r_ = r1;
    g_ = g1;
    b_ = b1;
    a_ = a1;
    return clamp();
}

std::shared_ptr<Color> Color::set(float r1, float g1, float b1) {
    r_ = r1;
    g_ = g1;
    b_ = b1;
    return clamp();
}

std::shared_ptr<Color> Color::set(int rgba) {
    return rgba8888(rgba);
}

float Color::sum() const {
    return r_ + g_ + b_;
}

std::shared_ptr<Color> Color::add(float r1, float g1, float b1, float a1) {
    r_ += r1;
    g_ += g1;
    b_ += b1;
    a_ += a1;
    return clamp();
}

std::shared_ptr<Color> Color::add(float r1, float g1, float b1) {
    r_ += r1;
    g_ += g1;
    b_ += b1;
    return clamp();
}

std::shared_ptr<Color> Color::sub(float r1, float g1, float b1, float a1) {
    r_ -= r1;
    g_ -= g1;
    b_ -= b1;
    a_ -= a1;
    return clamp();
}

std::shared_ptr<Color> Color::sub(float r1, float g1, float b1) {
    r_ -= r1;
    g_ -= g1;
    b_ -= b1;
    return clamp();
}

std::shared_ptr<Color> Color::inv() {
    r_ = 1.f - r_;
    g_ = 1.f - g_;
    b_ = 1.f - b_;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::r(float r1) {
    r_ = r1;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::g(float g1) {
    g_ = g1;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::b(float b1) {
    b_ = b1;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::a(float a1) {
    a_ = a1;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::mulA(float r1) {
    a_ *= r1;
    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::mul(float r1, float g1, float b1, float a1) {
    r_ *= r1;
    g_ *= g1;
    b_ *= b1;
    a_ *= a1;
    return clamp();
}

std::shared_ptr<Color> Color::lerp(const std::shared_ptr<Color>& target, float t) {
    r_ += t * (target->r_ - r_);
    g_ += t * (target->g_ - g_);
    b_ += t * (target->b_ - b_);
    a_ += t * (target->a_ - a_);
    return clamp();
}

std::shared_ptr<Color> Color::premultiplyAlpha() {
    r_ *= a_;
    g_ *= a_;
    b_ *= a_;

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::write(const std::shared_ptr<Color>& to) {
    return to->set(std::shared_ptr<Color>(this));
}

float Color::hue() const {
    toHsv(tmpHSV);
    return tmpHSV[0];
}

float Color::saturation() const {
    toHsv(tmpHSV);
    return tmpHSV[1];
}

float Color::value() const {
    toHsv(tmpHSV);
    return tmpHSV[2];
}

std::shared_ptr<Color> Color::hue(float amount) {
    toHsv(tmpHSV);
    tmpHSV[0] = amount;
    fromHsv(tmpHSV);

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::saturation(float amount) {
    toHsv(tmpHSV);
    tmpHSV[1] = amount;
    fromHsv(tmpHSV);

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::value(float amount) {
    toHsv(tmpHSV);
    tmpHSV[1] = amount;
    fromHsv(tmpHSV);

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::shiftHue(float amount) {
    toHsv(tmpHSV);
    tmpHSV[0] += amount;
    fromHsv(tmpHSV);

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::shiftSaturation(float amount) {
    toHsv(tmpHSV);
    tmpHSV[1] += amount;
    fromHsv(tmpHSV);

    return std::shared_ptr<Color>(this);
}

std::shared_ptr<Color> Color::shiftValue(float amount) {
    toHsv(tmpHSV);
    tmpHSV[1] += amount;
    fromHsv(tmpHSV);

    return std::shared_ptr<Color>(this);
}

bool Color::equals(const std::shared_ptr<Color>& o) {
    if (this == &*o)
        return true;
    if (!o)
        return false;
    return abgr() == o->abgr();
}

float Color::toFloatBits() const {
    std::uint32_t color = ((std::uint32_t)(255 * a_) << 24) | ((std::uint32_t)(255 * b_) << 16) | ((std::uint32_t)(255 * g_) << 8) | ((std::uint32_t)(255 * r_));
    return intToFloatColor(color);
}

double Color::toDoubleBits() const {
    return toDoubleBits(r_, g_, b_, a_);
}

std::uint32_t Color::abgr() const {
    return ((std::uint32_t)(255 * a_) << 24) | ((std::uint32_t)(255 * b_) << 16) | ((std::uint32_t)(255 * g_) << 8) | ((std::uint32_t)(255 * r_));
}

std::string Color::toString() const {
    std::string value;
    toString(value);
    return value;
}

void Color::toString(std::string &builder) const {
    std::stringstream stream;
    stream << std::hex << (((std::uint32_t)(255 * r_) << 24) | ((std::uint32_t)(255 * g_) << 16) | ((std::uint32_t)(255 * b_) << 8) | ((std::uint32_t)(255 * a_)));
    builder += stream.str();
    while (builder.length() < 8)
        builder.insert(builder.begin(), '0');
}

std::shared_ptr<Color> Color::fromHsv(float h, float s, float v) {
    auto x = (float)std::fmod((h / 60.f + 6), 6);
    int i = (int)x;
    float f = x - (float)i;
    float p = v * (1 - s);
    float q = v * (1 - s * f);
    float t = v * (1 - s * (1 - f));
    switch(i){
        case 0:
            r_ = v;
            g_ = t;
            b_ = p;
            break;
        case 1:
            r_ = q;
            g_ = v;
            b_ = p;
            break;
        case 2:
            r_ = p;
            g_ = v;
            b_ = t;
            break;
        case 3:
            r_ = p;
            g_ = q;
            b_ = v;
            break;
        case 4:
            r_ = t;
            g_ = p;
            b_ = v;
            break;
        default:
            r_ = v;
            g_ = p;
            b_ = q;
            break;
    }

    return clamp();
}

std::shared_ptr<Color> Color::fromHsv(float *value) {
    return fromHsv(value[0], value[1], value[2]);
}

float *Color::toHsv(float *hsv) const {
    float max = std::max(std::max(r_, g_), b_);
    float min = std::min(std::min(r_, g_), b_);
    float range = max - min;
    if(range == 0){
        hsv[0] = 0;
    }else if(max == r_){
        hsv[0] = (float)std::fmod((60 * (g_ - b_) / range + 360), 360);
    }else if(max == g_){
        hsv[0] = 60 * (b_ - r_) / range + 120;
    }else{
        hsv[0] = 60 * (r_ - g_) / range + 240;
    }

    if(max > 0){
        hsv[1] = 1 - min / max;
    }else{
        hsv[1] = 0;
    }

    hsv[2] = max;

    return hsv;
}

std::shared_ptr<Color> Color::HSVtoRGB(float h, float s, float v, float alpha) {
    std::shared_ptr<Color> c = HSVtoRGB(h, s, v);
    c->a_ = alpha;
    return c;
}

std::shared_ptr<Color> Color::HSVtoRGB(float h, float s, float v) {
    std::shared_ptr<Color> c = std::make_shared<Color>(1, 1, 1, 1);
    HSVtoRGB(h, s, v, c);
    return c;
}

std::shared_ptr<Color> Color::HSVtoRGB(float h, float s, float v, std::shared_ptr<Color> targetColor) {
    if(h == 360) h = 359;
    float r, g, b;
    int i;
    float f, p, q, t;
    h = (float) std::max(0.0f, std::min(360.0f, h));
    s = (float) std::max(0.0f, std::min(100.0f, s));
    v = (float) std::max(0.0f, std::min(100.0f, v));
    s /= 100;
    v /= 100;
    h /= 60;
    i = Mathf::floor(h);
    f = h - (float)i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch(i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:
            r = v;
            g = p;
            b = q;
    }

    targetColor->set(r, g, b, targetColor->a_);
    return targetColor;
}

std::vector<int> Color::RGBtoHSV(const std::shared_ptr<Color>& c) {
    return RGBtoHSV(c->r_, c->g_, c->b_);
}

std::vector<int> Color::RGBtoHSV(float r, float g, float b) {
    float h, s, v;
    float min, max, delta;

    min = std::min(std::min(r, g), b);
    max = std::max(std::max(r, g), b);
    v = max;

    delta = max - min;

    if(max != 0)
        s = delta / max;
    else {
        s = 0;
        h = 0;
        return {Mathf::round(h), Mathf::round(s), Mathf::round(v)};
    }

    if(delta == 0)
        h = 0;
    else {

        if(r == max)
            h = (g - b) / delta;
        else if(g == max)
            h = 2 + (b - r) / delta;
        else
            h = 4 + (r - g) / delta;
    }

    h *= 60;
    if(h < 0)
        h += 360;

    s *= 100;
    v *= 100;

    return {Mathf::round(h), Mathf::round(s), Mathf::round(v)};
}

std::shared_ptr<Color> Color::cpy() {
    return std::make_shared<Color>(*this);
}

std::shared_ptr<Color> Color::lerp(std::vector<std::shared_ptr<Color>> colors, float s) {
    std::size_t l = colors.size();
    std::shared_ptr<Color> a = colors[Mathf::clamp((int)(s * (float)(l - 1)), 0, (int)colors.size() - 1)];
    std::shared_ptr<Color> b = colors[Mathf::clamp((int)(s * (float)(l - 1) + 1), 0, (int)l - 1)];

    float n = s * (float)(l - 1) - (float)(int)(s * (float)(l - 1));
    float i = 1.f - n;
    return set(a->r_ * i + b->r_ * n, a->g_ * i + b->g_ * n, a->b_ * i + b->b_ * n, 1.f);
}

int Color::clampf(float value) {
    return std::min(std::max((int)value, 0), 255);
}

int Color::ri(float rgba) {
    return (int)((int)rgba & 0xff000000) >> 24;
}

int Color::gi(float rgba) {
    return (int)((int)rgba & 0x00ff0000) >> 16;
}

int Color::bi(float rgba) {
    return (int)((int)rgba & 0x0000ff00) >> 8;
}

int Color::ai(float rgba) {
    return (int)((int)rgba & 0xff);
}

int Color::muli(int ca, int cb) {
    int
    r = (int)((int)(ca & 0xff000000) >> 24),
    g = (int)((int)(ca & 0x00ff0000) >> 16),
    b = (int)((int)(ca & 0x0000ff00) >> 8),
    a = (int)((int)(ca & 0x000000ff)),
    r2 = (int)((int)(cb & 0xff000000) >> 24),
    g2 = (int)((int)(cb & 0x00ff0000) >> 16),
    b2 = (int)((int)(cb & 0x0000ff00) >> 8),
    a2 = (int)((int)(cb & 0x000000ff));
    return (clampf((float)r * (float)r2 / 255.f) << 24) |
           (clampf((float)g * (float)g2 / 255.f) << 16) |
           (clampf((float)b * (float)b2 / 255.f) << 8) |
           (clampf((float)a * (float)a2 / 255.f));
}

int Color::muli(int rgba, float value) {
    int
    r = (int)((int)(rgba & 0xff000000) >> 24),
    g = (int)((int)(rgba & 0x00ff0000) >> 16),
    b = (int)((int)(rgba & 0x0000ff00) >> 8),
    a = (int)((int)(rgba & 0x000000ff));
    return (clampf((float)r * value) << 24) |
           (clampf((float)g * value) << 16) |
           (clampf((float)b * value) << 8) |
           (a);
}
