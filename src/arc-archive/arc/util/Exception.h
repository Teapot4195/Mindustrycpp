//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_EXCEPTION_H
#define MINDUSTRY_SRC_ARC_UTIL_EXCEPTION_H

#include <iostream>
#include <stdexcept>

#include <boost/stacktrace.hpp>

class Throwable : public std::runtime_error {
private:
    boost::stacktrace::stacktrace trace;

public:
    explicit Throwable(const std::string& what_arg) : std::runtime_error(what_arg) {
        trace = boost::stacktrace::stacktrace();
    }
    explicit Throwable(const char* what_arg) : std::runtime_error(what_arg) {
        trace = boost::stacktrace::stacktrace();
    }
    explicit Throwable(const runtime_error& other) noexcept : std::runtime_error(other) {
        trace = boost::stacktrace::stacktrace();
    }

    [[nodiscard]] virtual std::string getStackTrace() const {
        std::string str;
        str += to_string(trace);
        return str;
    }
};

std::string unwrap_exception(const Throwable& e, int level = 0, std::string base = "");

std::string unwrap_exception(const std::exception& e, int level = 0, std::string base = "") {
    base += std::string(level, ' ');
    base += "exception: ";
    base += e.what();
    base += '\n';
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (const Throwable& nestedException)
    {
        unwrap_exception(nestedException, level + 1, base);
    }
    catch (const std::exception& nestedException)
    {
        unwrap_exception(nestedException, level + 1, base);
    }
    catch (...) {}
}

std::string unwrap_exception(const Throwable& e, int level, std::string base) {
    base += std::string(level, ' ');
    base += "exception: ";
    base += e.what();
    base += " at ";
    base += e.getStackTrace();
    base += '\n';
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (const Throwable& nestedException)
    {
        unwrap_exception(nestedException, level + 1, base);
    }
    catch (const std::exception& nestedException)
    {
        unwrap_exception(nestedException, level + 1, base);
    }
    catch (...) {}
}

#define basicException(Name) class Name : public Throwable { \
public:                                                      \
    using Throwable::Throwable;                              \
};

basicException(IOException)

basicException(ArcNotImplementedException)

basicException(ArcRuntimeException)


#endif //MINDUSTRY_SRC_ARC_UTIL_EXCEPTION_H
