//
// Created by teapot on 03/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_ARCNOTIMPLEMENTEDEXCEPTION_H
#define MINDUSTRY_SRC_ARC_UTIL_ARCNOTIMPLEMENTEDEXCEPTION_H

#include <stdexcept>

class ArcNotImplementedException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

#endif //MINDUSTRY_SRC_ARC_UTIL_ARCNOTIMPLEMENTEDEXCEPTION_H
