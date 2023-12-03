//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_EXCEPTION_H
#define MINDUSTRY_SRC_ARC_UTIL_EXCEPTION_H

#include <stdexcept>

#define basicException(Name) class Name : public std::runtime_error {public: using std::runtime_error::runtime_error;};

basicException(IOException)

basicException(ArcNotImplementedException)

basicException(ArcRuntimeException)


#endif //MINDUSTRY_SRC_ARC_UTIL_EXCEPTION_H
