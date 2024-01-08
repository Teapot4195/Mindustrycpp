//
// Created by teapot on 26/11/23.
//

#include "Application.h"

#include "Core.h"
#include "util/Time.h"

void Application::addListener(std::shared_ptr<ApplicationListener> listener) {
    auto l = getListeners();
    std::lock_guard<std::mutex> lockGuard(l.second);
    l.first.push_back(listener);
}

void Application::removeListener(std::shared_ptr<ApplicationListener> listener) {
    auto l = getListeners();
    std::lock_guard<std::mutex> lockGuard(l.second);
    std::erase(l.first, listener);
}

void Application::defaultUpdate() {
    Core::settings->autosave();
    Time::updateGlobal();
}

bool Application::isDesktop() {
    return getType() == ApplicationType::desktop;
}

bool Application::isHeadless() {
    return getType() == ApplicationType::headless;
}

bool Application::isAndroid() {
    return getType() == ApplicationType::android;
}

bool Application::isIOS() {
    return getType() == ApplicationType::iOS;
}

bool Application::isMobile() {
    return isAndroid() || isIOS();
}

bool Application::isWeb() {
    return getType() == ApplicationType::web;
}

int Application::getVersion() {
    return 0;
}

long long Application::getJavaHeap() {
    //TODO: Return actual memory in use
    return 0;
}

long long Application::getNAtiveHeap() {
    return 0;
}

bool Application::openFolder(std::string file) {
    return false;
}

bool Application::openURI(std::string URI) {
    return false;
}

Application::~Application() {
    if (Core::settings.has_value()) {
        Core::settings->autosave();
    }

    if (Core::audio.has_value()) {
        Core::audio.reset();
    }
}

void Application::dispose() {
    if (Core::settings.has_value()) {
        Core::settings->autosave();
    }

    if (Core::audio.has_value()) {
        Core::audio.reset();
    }
}
