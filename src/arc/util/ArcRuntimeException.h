//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_ARCRUNTIMEEXCEPTION_H
#define MINDUSTRY_SRC_ARC_UTIL_ARCRUNTIMEEXCEPTION_H

#include <stdexcept>

class ArcRuntimeException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};


#endif //MINDUSTRY_SRC_ARC_UTIL_ARCRUNTIMEEXCEPTION_H
