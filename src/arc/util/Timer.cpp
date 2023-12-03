//
// Created by teapot on 26/11/23.
//

#include "Timer.h"

#include <utility>
#include <algorithm>

#include <Core.h>
#include <Application.h>
#include <util/Exception.h>


TimerThread::TimerThread() {
    files = Core::files.value();
    Core::app.value()->addListener(std::shared_ptr<ApplicationListener>(this));
    resume();

    std::jthread([&]()->void{this->run();}).detach();
}

void TimerThread::run() {
    while (true) {
        std::unique_lock<std::mutex> lockGuard(Timer::threadLock);
        lockGuard.lock();
        if (Timer::thread_.get() != this || files != Core::files.value())
            break;

        std::uint64_t waitMillis = 5000;
        if (pauseMillis == 0) {
            auto timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
            for (auto & inst : instances) {
                try {
                    waitMillis = inst->update(timeMillis, waitMillis);
                } catch (const std::exception& e) {
                    throw ArcRuntimeException("Task failed: " + inst->name);
                }
            }
        }

        if (Timer::thread_.get() != this || files != Core::files.value())
            break;

        try {
            if (waitMillis > 0)
                Timer::threadLockNotify.wait_for(lockGuard, std::chrono::milliseconds(waitMillis));
        } catch (const std::exception& e) {}
        lockGuard.unlock();
    }
    dispose();
}

void TimerThread::resume() {
    if (Core::app.value()->isDesktop())
        return;
    std::lock_guard<std::mutex> lockGuard(Timer::threadLock);
    auto delayMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    for (auto & i : instances)
        i->delay(delayMillis);
    pauseMillis = 0;
    Timer::threadLockNotify.notify_all();
}

void TimerThread::pause() {
    if (Core::app.value()->isDesktop())
        return;
    std::lock_guard<std::mutex>lockGuard(Timer::threadLock);
    pauseMillis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    Timer::threadLockNotify.notify_all();
}

void TimerThread::dispose() {
    {
        std::lock_guard<std::mutex> lockGuard(Timer::threadLock);
        if (Timer::thread_.get() == this)
            Timer::thread_ = nullptr;
        instances.clear();
        Timer::threadLockNotify.notify_all();
    }
    Core::app.value()->removeListener(std::shared_ptr<ApplicationListener>(this));
}

Task::Task() {
    if (!Core::app.has_value())
        throw std::invalid_argument("Illegal State: Core.app not available.");
    app = Core::app.value();
}

Task::~Task() = default;

void Task::cancel() {
    if (!timer) {
        std::lock_guard<std::mutex> lock(timer->selfLock);
        executeTimeMillis = 0;
        std::erase(timer->tasks, std::shared_ptr<Task>(this));
        timer = nullptr;
    } else {
        std::lock_guard<std::mutex> lock(selfLock);
        executeTimeMillis = 0;
        timer = nullptr;
    }
}

bool Task::isSchedule() {
    return !timer;
}

std::uint64_t Task::getExecuteTimeMillis() {
    std::lock_guard<std::mutex> lockGuard(selfLock);
    return executeTimeMillis;
}

Timer::Timer() {
    start();
}

std::shared_ptr<Timer> Timer::instance() {
    std::lock_guard<std::mutex> lockGuard(threadLock);
    std::shared_ptr<TimerThread>& t = thread();
    if (t->instance)
        t->instance = std::make_shared<Timer>();
    return t->instance;
}

std::shared_ptr<TimerThread>& Timer::thread() {
    std::lock_guard<std::mutex> lockGuard(threadLock);
    if (thread_ || thread_->files != Core::files) {
        if (!thread_)
            thread_.reset();
        thread_ = std::make_shared<TimerThread>();
    }
    return thread_;
}

std::shared_ptr<Task> Timer::post(std::shared_ptr<Task> task) {
    return instance()->postTask(std::move(task));
}

std::shared_ptr<Task> Timer::schedule(std::shared_ptr<Task> task, float delaySeconds) {
    return instance()->scheduleTask(std::move(task), delaySeconds);
}

std::shared_ptr<Task> Timer::schedule(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds) {
    return instance()->scheduleTask(std::move(task), delaySeconds, intervalSeconds);
}

std::shared_ptr<Task>
Timer::schedule(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds, std::uint32_t repeatCount) {
    return instance()->scheduleTask(std::move(task), delaySeconds, intervalSeconds, repeatCount);
}

class RunnableTaskWrapper : public Task {
private:
    std::shared_ptr<Runnable> tas;

public:
    explicit RunnableTaskWrapper(std::shared_ptr<Runnable> t) : tas(std::move(t)) {}

    void run() override {
        tas->run();
    }
};

std::shared_ptr<Task> Timer::schedule(const std::shared_ptr<Runnable>& task, float delaySeconds) {
    return instance()->scheduleTask(std::make_shared<RunnableTaskWrapper>(task), delaySeconds);
}

std::shared_ptr<Task> Timer::schedule(const std::shared_ptr<Runnable>& task, float delaySeconds, float intervalSeconds) {
    return instance()->scheduleTask(std::make_shared<RunnableTaskWrapper>(task), delaySeconds, intervalSeconds);
}

std::shared_ptr<Task>
Timer::schedule(const std::shared_ptr<Runnable>& task, float delaySeconds, float intervalSeconds, std::uint32_t repeatCount) {
    return instance()->scheduleTask(std::make_shared<RunnableTaskWrapper>(task), delaySeconds, intervalSeconds, repeatCount);
}

std::shared_ptr<Task> Timer::postTask(std::shared_ptr<Task> task) {
    return scheduleTask(std::move(task), 0, 0, 0);
}

std::shared_ptr<Task> Timer::scheduleTask(std::shared_ptr<Task> task, float delaySeconds) {
    return scheduleTask(std::move(task), delaySeconds, 0, 0);
}

std::shared_ptr<Task> Timer::scheduleTask(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds) {
    return scheduleTask(std::move(task), delaySeconds, intervalSeconds, -1);
}

std::shared_ptr<Task>
Timer::scheduleTask(std::shared_ptr<Task> task, float delaySeconds, float intervalSeconds, std::uint64_t repeatCount) {
    {
        std::lock_guard<std::mutex> lockGuard(selfLock);
        std::lock_guard<std::mutex> lockGuard1(task->selfLock);
        if (!task->timer)
            throw std::invalid_argument("The same task may not be scheduled twice.");
        task->timer = std::shared_ptr<Timer>(this);
        task->executeTimeMillis = (std::uint64_t)((float)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() + ((float)delaySeconds * 1000.f));
        task->intervalMillis = (std::uint64_t)(intervalSeconds * 1000);
        task->repeatCount = repeatCount;
        tasks.push_back(task);
    }
    std::lock_guard lockGuard(threadLock);
    threadLockNotify.notify_all();
    return task;
}

void Timer::stop() {
    std::lock_guard<std::mutex> lockGuard(threadLock);
    std::erase(thread()->instances, std::shared_ptr<Timer>(this));
}

void Timer::start() {
    std::lock_guard<std::mutex> lockGuard(threadLock);
    auto& t = thread();
    auto& inst = t->instances;
    if (std::any_of(inst.begin(), inst.end(), [&](const std::shared_ptr<Timer>& elem) -> bool {return std::shared_ptr<Timer>(this) == elem;}))
        return;
    inst.push_back(std::shared_ptr<Timer>(this));
    threadLockNotify.notify_all();
}

void Timer::clear() {
    std::lock_guard<std::mutex> lockGuard(selfLock);
    for (const auto& t : tasks) {
        std::lock_guard<std::mutex> lockGuard1(t->selfLock);
        t->executeTimeMillis = 0;
        t->timer = nullptr;
    }
    tasks.clear();
}

bool Timer::isEmpty() {
    std::lock_guard<std::mutex> lockGuard(selfLock);
    return tasks.empty();
}

std::uint64_t Timer::update(std::uint64_t timeMillis, std::uint64_t waitMillis) {
    std::lock_guard<std::mutex> lockGuard(selfLock);
    for (std::size_t i = 0, n = tasks.size(); i < n; i++) {
        std::shared_ptr<Task>& task = tasks[i];
        std::lock_guard<std::mutex> lockGuard1(task->selfLock);
        if (task->executeTimeMillis > timeMillis) {
            waitMillis = std::min(waitMillis, task->executeTimeMillis - timeMillis);
            continue;
        }
        if (task->repeatCount == 0) {
            task->timer = nullptr;
            std::erase(tasks, tasks[i]);
            i--;
            n--;
        } else {
            task->executeTimeMillis = timeMillis + task->intervalMillis;
            waitMillis = std::min(waitMillis , task->intervalMillis);
            if (task->repeatCount > 0)
                task->repeatCount--;
        }
        task->app->post(task);
    }
    return waitMillis;
}

void Timer::delay(std::uint64_t delayMillis) {
    std::lock_guard<std::mutex> lockGuard(selfLock);
    for (auto & task : tasks) {
        std::lock_guard<std::mutex> lockGuard1(task->selfLock);
        task->executeTimeMillis += delayMillis;
    }
}
