//
// Created by teapot on 25/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_TIME_H
#define MINDUSTRY_SRC_ARC_UTIL_TIME_H

#include <memory>
#include <queue>

#include <arc/struct.h>
#include <arc/func.h>
#include <arc/util/pooling/Pool.h>
#include <arc/util/pooling/Pools.h>

// Forward decl to avoid circular reference
class Task;

class DelayRun : public Poolable {
public:
    float delay;
    std::shared_ptr<Runnable> finish;

    void reset() override;
};

class Time {
public:

    constexpr static const float toSeconds = 60.f, toMinutes = 60.f * 60.f, toHours = 60.f * 60.f * 60.f;

    static float delta;

    static float time, globalTime;

    constexpr static const long nanosPerMilli = 1000000;

private:
    static double timeRaw, globalTimeRaw;
    static Seq<std::shared_ptr<DelayRun>> runs;
    static Seq<std::shared_ptr<DelayRun>> removal;

    static std::deque<long long> marks;

    static Floatp deltaimpl;

public:
    static void run(float delay, std::shared_ptr<Runnable> r);

    static std::shared_ptr<Task> runTask(float delay, std::shared_ptr<Runnable> r);

    static void mark();

    static float elapsed();

    static void updateGlobal();

    static void clear();

    static void setDeltaProvider(const Floatp& impl);

    static std::uint64_t nanos();

    static std::uint64_t millis();

    static std::uint64_t nanosToMillis(std::uint64_t nanos);

    static std::uint64_t millisToNanos(std::uint64_t millis);

    static std::uint64_t timeSinceNanos(std::uint64_t prevTime);

    static std::uint64_t timeSinceMillis(std::uint64_t prevTime);
};


#endif //MINDUSTRY_SRC_ARC_UTIL_TIME_H
