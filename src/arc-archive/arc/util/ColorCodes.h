//
// Created by teapot on 05/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_COLORCODES_H
#define MINDUSTRY_SRC_ARC_UTIL_COLORCODES_H

#include <string>
#include <vector>

#include <arc/Utils.h>

class ColorCodes {
public:
    static std::string
        flush, reset, bold, italic, underline, black, red, green, yellow, blue, purple, cyan, lightBlack, lightRed,
        lightGreen, lightYellow, lightBlue, lightMagenta, lightCyan, lightWhite, white, backDefault, backRed,
        backGreen, backYellow, backBlue;

    static std::vector<std::string> codes, values;

    sblockfdcl()
};


#endif //MINDUSTRY_SRC_ARC_UTIL_COLORCODES_H
