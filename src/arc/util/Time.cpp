//
// Created by teapot on 25/11/23.
//

#include "Time.h"

#include <utility>
#include <cmath>

#include <Core.h>
#include "Timer.h"

float Time::delta = 1.f;

Seq<std::shared_ptr<DelayRun>> Time::runs = Seq<std::shared_ptr<DelayRun>>();

Seq<std::shared_ptr<DelayRun>> Time::removal = Seq<std::shared_ptr<DelayRun>>();

LongSeq Time::marks = LongSeq();

Floatp Time::deltaimpl = []() -> float {
    return std::min(Core::graphics::getDeltaTime() * 60.f, 3.f);
};

void DelayRun::reset() {
    delay = 0;
    finish = nullptr;
}

void Time::run(float delay, std::shared_ptr<Runnable> r) {
    std::shared_ptr<DelayRun> run = Pools<DelayRun>::obtain([]()->std::shared_ptr<DelayRun>{
        return std::make_shared<DelayRun>();
    });
    run->finish = std::move(r);
    run->delay = delay;
    runs.push_back(run);
}

std::shared_ptr<Task> Time::runTask(float delay, std::shared_ptr<Runnable> r) {
    return Timer::schedule(r, delay / 60.f);
}

void Time::mark() {
    marks.push_back(nanos());
}

float Time::elapsed() {
    if (marks.empty())
        return -1;
    else {
        auto f = marks.front();
        marks.pop_front();
        return (float)timeSinceNanos(f) / 1000000.f;
    }
}

void Time::updateGlobal() {
    globalTimeRaw += Core::graphics.value()->getDeltaTime()*60.f;
    delta = deltaimpl();

    if (std::isinf(timeRaw) || std::isnan(timeRaw))
        timeRaw = 0;

    time = (float)timeRaw;
    globalTime = (float)globalTimeRaw;

    for (auto& run : runs) {
        run->delay -= delta;

        if (run->delay <= 0) {
            run->finish->run();
            removal.push_back(run);
            Pools<DelayRun>::free(run);
        }
    }
}

void Time::clear() {
    runs.clear();
}

void Time::setDeltaProvider(const Floatp& impl) {
    deltaimpl = impl;
    delta = impl();
}

std::uint64_t Time::nanos() {
    return std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
}

std::uint64_t Time::millis() {
    return std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();
}

std::uint64_t Time::nanosToMillis(std::uint64_t nanos) {
    return nanos / nanosPerMilli;
}

std::uint64_t Time::millisToNanos(std::uint64_t millis) {
    return millis * nanosPerMilli;
}

std::uint64_t Time::timeSinceNanos(std::uint64_t prevTime) {
    return nanos() - prevTime;
}

std::uint64_t Time::timeSinceMillis(std::uint64_t prevTime) {
    return millis() - prevTime;
}
