//
// Created by teapot on 26/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_TIMER_H
#define MINDUSTRY_SRC_ARC_UTIL_TIMER_H

#include <mutex>
#include <thread>
#include <condition_variable>
#include <optional>

#include <struct.h>
#include <ApplicationListener.h>

// Resolve circular dependency with forward decls
class Timer;
class Files;
class Application;

class TimerThread : public Runnable, public ApplicationListener {
public:
    std::shared_ptr<Files> files;
    Seq<std::shared_ptr<Timer>> instances = Seq<std::shared_ptr<Timer>>(1);
    std::shared_ptr<Timer> instance;

private:
    long pauseMillis{};

public:
    TimerThread();

    void run() override;

    void resume() override;

    void pause() override;

    void dispose() override;
};

class Task : public Runnable {
public:
    std::mutex selfLock;
    std::shared_ptr<Application> app;
    std::uint64_t executeTimeMillis{}, intervalMillis{};
    std::uint32_t repeatCount{};
    std::shared_ptr<Timer> timer;

    Task();

    ~Task() override;

    void run() override = 0;

    virtual void cancel();

    virtual bool isSchedule();

    virtual std::uint64_t getExecuteTimeMillis();
};

class Timer {
public:
    static std::mutex threadLock;
    static std::condition_variable threadLockNotify;
    static std::shared_ptr<TimerThread> thread_;

    Seq<std::shared_ptr<Task>> tasks = Seq<std::shared_ptr<Task>>(8);

    std::mutex selfLock;
    std::string name;

    Timer();

    static std::shared_ptr<Timer> instance();

    static std::shared_ptr<TimerThread>& thread();

    static std::shared_ptr<Task> post(std::shared_ptr<Task> task);

    static std::shared_ptr<Task> schedule(std::shared_ptr<Task> task, float delaySeconds);

    static std::shared_ptr<Task> schedule(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds);

    static std::shared_ptr<Task> schedule(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds, std::uint32_t repeatCount);

    static std::shared_ptr<Task> schedule(const std::shared_ptr<Runnable>& task, float delaySeconds);

    static std::shared_ptr<Task> schedule(const std::shared_ptr<Runnable>& task, float delaySeconds, float intervalSeconds);

    static std::shared_ptr<Task> schedule(const std::shared_ptr<Runnable>& task, float delaySeconds, float intervalSeconds, std::uint32_t repeatCount);

    std::shared_ptr<Task> postTask(std::shared_ptr<Task> task);

    std::shared_ptr<Task> scheduleTask(std::shared_ptr<Task> task, float delaySeconds);

    std::shared_ptr<Task> scheduleTask(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds);

    std::shared_ptr<Task> scheduleTask(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds, std::uint64_t repeatCount);

    void stop();

    void start();

    void clear();

    bool isEmpty();

    std::uint64_t update(std::uint64_t timeMillis, std::uint64_t waitMillis);

    void delay(std::uint64_t delayMillis);
};


#endif //MINDUSTRY_SRC_ARC_UTIL_TIMER_H
