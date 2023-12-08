//
// Created by teapot on 05/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_LOG_H
#define MINDUSTRY_SRC_ARC_UTIL_LOG_H

#include <format>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <arc/util/Exception.h>
#include <arc/Utilities.h>

enum class LogLevel {
    debug,
    info,
    warn,
    err,
    none
};

using LogFormatter = std::function<std::string(std::string, bool, std::format_args)>;

std::string DefaultFormat(std::string text, bool useColors, std::format_args args);

LogFormatter DefaultLogFormatter = DefaultFormat;

using LogHandler = std::function<void(LogLevel, std::string)>;

extern LogHandler DefaultLogHandler;

extern LogHandler NoopLogHandler;

class Log {
public:
    static bool useColors;
    static LogLevel level;
    static LogHandler logger;
    static LogFormatter formatter;

    static void log(LogLevel lev, std::string text, std::format_args args = std::make_format_args());

    static void debug(std::string text, std::format_args args);

    static void debug(const std::shared_ptr<Stringable>& object);

    static void debug(std::string str);

    static void infoList(std::initializer_list<std::shared_ptr<Stringable>> args);

    static void infoTag(const std::string& tag, const std::string& text);

    static void info(std::string text, std::format_args args);

    static void info(const std::shared_ptr<Stringable>& object);

    static void info(std::string str);

    static void warn(std::string text, std::format_args args = std::make_format_args());

    static void errTag(const std::string& tag, const std::string& text);

    static void err(std::string text, std::format_args args = std::make_format_args());

    static void err(const Throwable& e);

    static void err(const std::exception& e);

    static void err(const std::string& text, const Throwable& e);

    static void err(const std::string& text, const std::exception& e);

    static std::string format(std::string text, std::format_args args = std::make_format_args());

    static std::string formatColors(std::string text, bool useCol, std::format_args args = std::make_format_args());

    static std::string removeColors(std::string text);

    static std::string addColors(std::string text);
};

std::string DefaultFormat(std::string text, bool useColors, std::format_args args = std::make_format_args()) {
    text = std::vformat(text, args);
    return useColors ? Log::addColors(text) : Log::removeColors(text);
}

#endif //MINDUSTRY_SRC_ARC_UTIL_LOG_H
