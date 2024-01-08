//
// Created by teapot on 22/11/23.
//

#include "Mathf.h"

#include "geom/Vec2.h"
#include "geom/Position.h"
#include "util/Time.h"

const float Mathf::sqrt2 = Mathf::sqrt(2.f);
const float Mathf::sqrt3 = Mathf::sqrt(3.f);

Rand Mathf::seedr = Rand();
Rand Mathf::rand = Rand();

float* Mathf::sinTable = []() -> float* {
    auto* ret = new float[sinCount];
    for (std::size_t i = 0; i < sinCount; i++)
        ret[i] = std::sin(((float) i + 0.5f) / sinCount * radFull);
    for (std::size_t i = 0 ; i < 360; i += 90)
        sinTable[(int)((float)i * degToIndex) & sinMask] = (float)std::sin((float)i * degreesToRadians);

    sinTable[0] = 0.f;
    sinTable[(int)(90 * degToIndex) & sinMask] = 1.f;
    sinTable[(int)(180 * degToIndex) & sinMask] = 0.f;
    sinTable[(int)(270 * degToIndex) & sinMask] = -1.f;

    return ret;
}();

const std::shared_ptr<Vec2> Mathf::v1 = std::make_shared<Vec2>();
const std::shared_ptr<Vec2> Mathf::v2 = std::make_shared<Vec2>();
const std::shared_ptr<Vec2> Mathf::v3 = std::make_shared<Vec2>();

float Mathf::sin(float radians) {
    return sinTable[(int)(radians * radToIndex) & sinMask];
}

float Mathf::cos(float radians) {
    return sinTable[(int)((radians + PI / 2) * radToIndex) & sinMask];
}

float Mathf::sinDeg(float degrees) {
    return sinTable[(int)(degrees * degToIndex) & sinMask];
}

float Mathf::cosDeg(float degrees) {
    return sinTable[(int)((degrees + 90) * degToIndex) & sinMask];
}

float Mathf::absin(float scl, float mag) {
    return absin(Time::time, scl, mag);
}

float Mathf::absin(float in, float scl, float mag) {
    return (sin(in, scl * 2.f, mag) + mag) / 2.f;
}

float Mathf::tan(float radians, float scl, float mag) {
    return (sin(radians / scl)) / (cos(radians / scl)) * mag;
}

float Mathf::sin(float scl, float mag) {
    return sin(Time::time / scl) * mag;
}

float Mathf::sin(float radians, float scl, float mag) {
    return sin(radians / scl) * mag;
}

float Mathf::cos(float radians, float scl, float mag) {
    return cos(radians / scl) * mag;
}

float Mathf::angle(float x, float y) {
    float result = atan2(x, y) * radDeg;
    if (result < 0)
        result += 360.f;
    return result;
}

float Mathf::angleExact(float x, float y) {
    float result = std::atan2(y, x) * radDeg;
    if (result < 0)
        result += 360.f;
    return result;
}

float Mathf::wrapAngleAroundZero(float a) {
    if (a >= 0) {
        float rotation = std::fmod(a, Mathf::PI2);
        if (rotation > Mathf::PI)
            rotation -= Mathf::PI2;
        return rotation;
    } else {
        float rotation = std::fmod(-a, Mathf::PI2);
        if (rotation > Mathf::PI)
            rotation -= Mathf::PI2;
        return -rotation;
    }
}

float Mathf::atn(double i) {
    double n = std::abs(i);
    double c = (n - 1.0) / (n + 1.0);
    double c2 = c * c, c3 = c * c2, c5 = c3 * c2, c7 = c5 * c2, c9 = c7 * c2, c11 = c9 * c2;
    return (float)std::copysign((M_PI * 0.25)
        + (0.99997726 * c - 0.33262347 * c3 + 0.19354346 * c5 - 0.11643287 * c7 + 0.05265332 * c9 - 0.0117212 * c11), i);
}

float Mathf::atan2(float x, float y) {
    float n = y / x;
    if(std::isinf(n)) {
        n = (y == x ? 1.f : -1.f); // if both y and x are infinite, n would be NaN
    } else if(std::isinf(n - n)) {
        x = 0.f; // if n is infinite, y is infinitely larger than x.
    }

    if(x > 0) {
        return atn(n);
    } else if(x < 0) {
        return y >= 0 ? atn(n) + PI : atn(n) - PI;
    } else if(y > 0) {
        return x + halfPi;
    } else if(y < 0) {
        return x - halfPi;
    } else {
        return x + y; // returns 0 for 0,0 or NaN if either y or x is NaN
    }
}

int Mathf::digits(int n) {
    return n < 100000 ? n < 100 ? n < 10 ? 1 : 2 : n < 1000 ? 3 : n < 10000 ? 4 : 5 : n < 10000000 ? n < 1000000 ? 6 : 7 : n < 100000000 ? 8 : n < 1000000000 ? 9 : 10;
}

int Mathf::digits(long long int n) {
    return n == 0 ? 1 : (int)(std::log10(n) + 1);
}

float Mathf::sqrt(float x) {
    return std::sqrt(x);
}

float Mathf::sqr(float x) {
    return x * x;
}

float Mathf::map(float value, float froma, float toa, float fromb, float tob) {
    return fromb + (value - froma) * (tob - fromb) / (toa - froma);
}

float Mathf::map(float value, float from, float to) {
    return map(value, 0, 1, from, to);
}

int Mathf::sign(float f) {
    return (f < 0 ? -1 : 1);
}

int Mathf::sign(bool b) {
    return b ? 1 : -1;
}

int Mathf::num(bool b) {
    return b ? 1 : 0;
}

float Mathf::pow(float a, float b) {
    return std::pow(a, b);
}

int Mathf::pow(int a, int b) {
    return std::ceil(std::pow(a, b));
}

float Mathf::range(float range) {
    return random(-range, range);
}

int Mathf::range(int range) {
    return random(-range, range);
}

float Mathf::range(float min, float max) {
    if (chance(0.5)) {
        return random(min, max);
    } else {
        return -random(min, max);
    }
}

bool Mathf::chanceDelta(double d) {
    return rand.nextFloat() < d * Time::delta;
}

bool Mathf::chance(double d) {
    return d >= 1.f || rand.nextFloat() < d;
}

int Mathf::random(int range) {
    return (int)rand.nextInt(range + 1);
}

int Mathf::random(int start, int end) {
    return start + (int)rand.nextInt(end - start + 1);
}

long long Mathf::random(long long range) {
    return (int)rand.nextDouble() * range;
}

long long Mathf::random(long long int start, long long int end) {
    return start + ((int)rand.nextDouble() * (end - start));
}

bool Mathf::randomBoolean() {
    return rand.nextBoolean();
}

bool Mathf::randomBoolean(float chance) {
    return Mathf::random() < chance;
}

float Mathf::random() {
    return rand.nextFloat();
}

float Mathf::random(float range) {
    return rand.nextFloat() * range;
}

float Mathf::random(float start, float end) {
    return start + rand.nextFloat() * (end - start);
}

int Mathf::randomSign() {
    return (int)(1 | (rand.nextInt() >> 31));
}

int Mathf::randomSeed(long long int seed, int min, int max) {
    seedr.setSeed(seed);
    if (isPowerOfTwo(max))
        seedr.nextInt();
    return (int)seedr.nextInt(max - min + 1) + min;
}

float Mathf::randomSeed(long long int seed, float min, float max) {
    seedr.setSeed(seed);
    return (min + seedr.nextFloat() * (max - min));
}

float Mathf::randomSeed(long long int seed) {
    seedr.setSeed(seed * 99999);
    return seedr.nextFloat();
}

float Mathf::randomSeed(long long int seed, float max) {
    seedr.setSeed(seed * 99999);
    return seedr.nextFloat() * max;
}

float Mathf::randomSeedRange(long long int seed, float range) {
    seedr.setSeed(seed * 99999);
    return range * (seedr.nextFloat() - 0.5f) * 2.f;
}

float Mathf::randomTriangular() {
    return rand.nextFloat() - rand.nextFloat();
}

float Mathf::randomTriangular(float max) {
    return (rand.nextFloat() - rand.nextFloat()) * max;
}

float Mathf::randomTriangular(float min, float max) {
    return randomTriangular(min, max, (min + max) * 0.5f);
}

float Mathf::randomTriangular(float min, float max, float mode) {
    float u = rand.nextFloat();
    float d = max - min;
    if (u <= (mode - min) / d)
        return min + std::sqrt(u * d * (mode - min));
    return max - std::sqrt((1 - u) * d * (max - mode));
}

int Mathf::nextPowerOfTwo(int value) {
    if (value == 0)
        return 1;
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return value + 1;
}

bool Mathf::isPowerOfTwo(int value) {
    return value != 0 && (value & value - 1) == 0;
}

int Mathf::clamp(int value, int min, int max) {
    return std::max(std::min(value, max), min);
}

long long Mathf::clamp(long long int value, long long int min, long long int max) {
    return std::max(std::min(value, max), min);
}

float Mathf::clamp(float value, float min, float max) {
    return std::max(std::min(value, max), min);
}

float Mathf::clamp(float value) {
    return clamp(value, 0.f, 1.f);
}

double Mathf::clamp(double value, double min, double max) {
    return std::max(std::min(value, max), min);
}

float Mathf::maxZero(float val) {
    return std::max(val, 0.f);
}

float Mathf::approach(float from, float to, float speed) {
    return from + Mathf::clamp(to - from, -speed, speed);
}

float Mathf::approachDelta(float from, float to, float speed) {
    return approach(from, to, Time::delta * speed);
}

float Mathf::lerp(float fromValue, float toValue, float progress) {
    return fromValue + (toValue - fromValue) * progress;
}

float Mathf::lerpDelta(float fromValue, float toValue, float progress) {
    return lerp(fromValue, toValue, clamp(progress * Time::delta));
}

float Mathf::slerpRad(float fromRadians, float toRadians, float progress) {
    float delta = std::fmod((toRadians - fromRadians + PI2 + PI), PI2) - PI;
    return std::fmod((fromRadians + delta * progress + PI2), PI2);
}

float Mathf::slerp(float fromDegrees, float toDegrees, float progress) {
    float delta = (float)std::fmod((toDegrees - fromDegrees + 360 + 180), 360) - 180;
    return (float)std::fmod((fromDegrees + delta * progress + 360), 360);
}

float Mathf::slerpDelta(float fromDegrees, float toDegrees, float progress) {
    return slerp(fromDegrees, toDegrees, clamp(progress * Time::delta));
}

int Mathf::floor(float value) {
    return (int)(value + BIG_ENOUGH_FLOOR) - BIG_ENOUGH_INT;
}

int Mathf::floorPositive(float value) {
    return (int)value;
}

int Mathf::ceil(float value) {
    return BIG_ENOUGH_INT - (int)(BIG_ENOUGH_FLOOR - value);
}

int Mathf::ceilPositive(float value) {
    return (int)(value + CEIL);
}

int Mathf::round(float value) {
    return (int)(value + BIG_ENOUGH_ROUND) - BIG_ENOUGH_INT;
}

int Mathf::round(int value, int step) {
    return (value / step) * step;
}

float Mathf::round(float value, float step) {
    return (float)(value / step) * step;
}

int Mathf::round(float value, int step) {
    return (int)(value / (float)step) * step;
}

int Mathf::roundPositive(float value) {
    return (int)(value + 0.5f);
}

bool Mathf::zero(float value) {
    return std::abs(value) <= FLOAT_ROUNDING_ERROR;
}

bool Mathf::zero(double value) {
    return std::abs(value) <= FLOAT_ROUNDING_ERROR;
}

bool Mathf::zero(float value, float tolerance) {
    return std::abs(value) <= tolerance;
}

bool Mathf::equal(float a, float b) {
    return std::abs(a - b) <= FLOAT_ROUNDING_ERROR;
}

bool Mathf::equal(float a, float b, float tolerance) {
    return std::abs(a - b) <= tolerance;
}

float Mathf::log(float a, float value) {
    return std::log(value) / std::log(a);
}

float Mathf::log2(float value) {
    return log(2, value);
}

int Mathf::log2(int value) {
    return value == 0 ? 0 : 31 - std::countl_zero((unsigned int)std::abs(value));
}

float Mathf::mod(float f, float n) {
    return std::fmod((std::fmod(f, n) + n), n);
}

int Mathf::mod(int x, int n) {
    return ((x % n) + n) % n;
}

float Mathf::sample(std::vector<float> values, float time) {
    time = clamp(time);
    float pos = time * (float)(values.size() - 1);
    int cur = std::min((int)(time * (float)(values.size() - 1)) , (int)values.size() - 1);
    int next = std::min(cur + 1, (int)values.size() - 1);
    float mod = (pos - (float)cur);
    return lerp(values[cur], values[next], mod);
}

float Mathf::slope(float fin) {
    return 1.f - std::abs(fin - 0.5f) * 2.f;
}

float Mathf::curve(float f, float offset) {
    if (f < offset)
        return 0.f;
    else
        return (f - offset) / (1.f - offset);
}

float Mathf::curve(float f, float from, float to) {
    if (f < from)
        return 0.f;
    else if (f > to)
        return 1.f;
    else
        return (f - from) / (to - from);
}

float Mathf::curveMargin(float f, float margin) {
    return curveMargin(f, margin, margin);
}

float Mathf::curveMargin(float f, float marginLeft, float marginRight) {
    if (f < marginLeft)
        return f / marginLeft * 0.5f;
    if (f > 1.f - marginRight)
        return (f - 1.f + marginRight) / marginRight * 0.5f + 0.5f;
    return 0.5f;
}

float Mathf::len(float x, float y) {
    return std::sqrt(x * x + y * y);
}

float Mathf::len2(float x, float y) {
    return x * x + y * y;
}

float Mathf::dot(float x1, float y1, float x2, float y2) {
    return x1 * x2 + y1 * y2;
}

float Mathf::dst(float x1, float y1) {
    return std::sqrt(x1 * x1 + y1 * y1);
}

float Mathf::dst2(float x1, float y1) {
    return x1 * x1 + y1 * y1;
}

float Mathf::dst(float x1, float y1, float x2, float y2) {
    float xd = x2 - x1;
    float yd = y2 - y1;
    return std::sqrt(xd * xd + yd * yd);
}

float Mathf::dst2(float x1, float y1, float x2, float y2) {
    float xd = x2 - x1;
    float yd = y2 - y1;
    return xd * xd + yd * yd;
}

float Mathf::dstm(float x1, float y1, float x2, float y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::shared_ptr<Vec2>
Mathf::arrive(const std::shared_ptr<Position>& pos, const std::shared_ptr<Position>& target, const std::shared_ptr<Vec2>& curVel,
              float radius, float tolerance, float speed, float smoothTime) {
    return arrive(pos->getX(), pos->getY(), target->getX(), target->getY(), curVel, radius, tolerance, speed, smoothTime);
}

std::shared_ptr<Vec2>
Mathf::arrive(float x, float y, float destX, float destY, const std::shared_ptr<Vec2>& curVel, float radius, float tolerance,
              float speed, float accel) {
    std::shared_ptr<Vec2> toTarget = v1->set(destX, destY)->sub(x, y);
    float distance = toTarget->len();

    if (distance <= tolerance)
        return v3->setZero();
    float targetSpeed = speed;
    if (distance <= radius)
        targetSpeed *= distance / radius;

    return toTarget->sub(curVel->x / accel, curVel->y / accel)->limit(targetSpeed);
}

bool Mathf::within(float x1, float y1, float x2, float y2, float dst) {
    return dst2(x1, y1, x2, y2) < dst * dst;
}

bool Mathf::within(float x1, float y1, float dst) {
    return x1 * x1 + y1 * y1 < dst * dst;
}
