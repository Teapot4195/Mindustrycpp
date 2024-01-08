//
// Created by teapot on 05/12/23.
//

#include "Log.h"

#include <iostream>
#include <utility>

#include <arc/util/ColorCodes.h>

LogHandler DefaultLogHandler = [](LogLevel level, const std::string& text) -> void {
    std::cout <<
        (Log::format(((
                level == LogLevel::debug ? "&lc&fb" :
                level == LogLevel::info ? "&fb" :
                level == LogLevel::warn ? "&ly&fb" :
                level == LogLevel::err ? "&lr&fb" :
                "") + text + "&fr"), std::make_format_args())) <<
    std::endl;
};

LogHandler NoopLogHandler = [](LogLevel level, const std::string& text) -> void {};

bool Log::useColors = true;

LogLevel Log::level = LogLevel::info;

LogHandler Log::logger = DefaultLogHandler;

LogFormatter Log::formatter = DefaultLogFormatter;

void Log::log(LogLevel lev, std::string text, std::format_args args) {
    if (Log::level > lev)
        return;
    logger(lev, format(std::move(text), args));
}

void Log::debug(std::string text, std::format_args args) {
    log(LogLevel::debug, std::move(text), args);
}

void Log::debug(const std::shared_ptr<Stringable>& object) {
    debug(object->toString());
}

void Log::debug(std::string str) {
    log(LogLevel::debug, std::move(str));
}

void Log::infoList(std::initializer_list<std::shared_ptr<Stringable>> args) {
    if (level > LogLevel::info)
        return;
    std::string builder;
    for (auto &e : args) {
        builder += e->toString();
        builder += " ";
    }
    info(builder);
}

void Log::infoTag(const std::string& tag, const std::string& text) {
    log(LogLevel::info, '[' + tag + ']' + text);
}

void Log::info(std::string text, std::format_args args) {
    log(LogLevel::info, std::move(text), args);
}

void Log::info(const std::shared_ptr<Stringable>& object) {
    info(object->toString());
}

void Log::info(std::string str) {
    log(LogLevel::info, std::move(str));
}

void Log::warn(std::string text, std::format_args args) {
    log(LogLevel::warn, std::move(text), args);
}

void Log::errTag(const std::string& tag, const std::string& text) {
    log(LogLevel::err, '[' + tag + ']' + text);
}

void Log::err(std::string text, std::format_args args) {
    log(LogLevel::err, std::move(text), args);
}

void Log::err(const Throwable &e) {
    err(unwrap_exception(e));
}

void Log::err(const std::exception &e) {
    err(unwrap_exception(e));
}

void Log::err(const std::string& text, const Throwable &e) {
    err(text + ": " + unwrap_exception(e));
}

void Log::err(const std::string& text, const std::exception &e) {
    err(text + ": " + unwrap_exception(e));
}

std::string Log::format(std::string text, std::format_args args) {
    return formatColors(std::move(text), useColors, args);
}

std::string Log::formatColors(std::string text, bool useCol, std::format_args args) {
    return formatter(std::move(text), useCol, args);
}

std::string Log::removeColors(std::string text) {
    for(std::string& col: ColorCodes::codes) {
        std::string from = "&" + col, to;
        size_t start_pos = 0;
        while((start_pos = text.find(from, start_pos)) != std::string::npos) {
            text.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    return text;
}

std::string Log::addColors(std::string text) {
    for (int i = 0; i < ColorCodes::codes.size(); i++){
        std::string from = "&" + ColorCodes::codes[i], to = ColorCodes::values[i];
        size_t start_pos = 0;
        while((start_pos = text.find(from, start_pos)) != std::string::npos) {
            text.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
    return text;
}
