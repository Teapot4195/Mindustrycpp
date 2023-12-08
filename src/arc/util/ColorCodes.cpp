//
// Created by teapot on 05/12/23.
//

#include "ColorCodes.h"

#include <arc/util/OS.h>

std::string ColorCodes::flush = "\033[H\033[2J";
std::string ColorCodes::reset = "\u001B[0m";
std::string ColorCodes::bold = "\u001B[1m";
std::string ColorCodes::italic = "\u001B[3m";
std::string ColorCodes::underline = "\u001B[4m";
std::string ColorCodes::black = "\u001B[30m";
std::string ColorCodes::red = "\u001B[31m";
std::string ColorCodes::green = "\u001B[32m";
std::string ColorCodes::yellow = "\u001B[33m";
std::string ColorCodes::blue = "\u001B[34m";
std::string ColorCodes::purple = "\u001B[35m";
std::string ColorCodes::cyan = "\u001B[36m";
std::string ColorCodes::lightBlack = "\u001b[90m";
std::string ColorCodes::lightRed = "\u001B[91m";
std::string ColorCodes::lightGreen = "\u001B[92m";
std::string ColorCodes::lightYellow = "\u001B[93m";
std::string ColorCodes::lightBlue = "\u001B[94m";
std::string ColorCodes::lightMagenta = "\u001B[95m";
std::string ColorCodes::lightCyan = "\u001B[96m";
std::string ColorCodes::lightWhite = "\u001b[97m";
std::string ColorCodes::white = "\u001B[37m";
std::string ColorCodes::backDefault = "\u001B[49m";
std::string ColorCodes::backRed = "\u001B[41m";
std::string ColorCodes::backGreen = "\u001B[42m";
std::string ColorCodes::backYellow = "\u001B[43m";
std::string ColorCodes::backBlue = "\u001B[44m";

sblock(ColorCodes, ({
    if((OS::isWindows && !OS::hasEnv("WT_SESSION")) || OS::isAndroid){
        flush = reset = bold = underline = black = red = green = yellow = blue = purple = cyan = lightWhite
                = lightBlack = lightRed = lightGreen = lightYellow = lightBlue = lightMagenta = lightCyan
                = white = backDefault = backRed = backYellow = backBlue = backGreen = italic = "";
    }

    codes = {
            "ff",
            "fr",
            "fb",
            "fi",
            "fu",
            "k",
            "lk",
            "lw",
            "r",
            "g",
            "y",
            "b",
            "p",
            "c",
            "lr",
            "lg",
            "ly",
            "lm",
            "lb",
            "lc",
            "w",

            "bd",
            "br",
            "bg",
            "by",
            "bb"
    };

    values = {
            flush,
            reset,
            bold,
            italic,
            underline,
            black,
            lightBlack,
            lightWhite,
            red,
            green,
            yellow,
            blue,
            purple,
            cyan,
            lightRed,
            lightGreen,
            lightYellow,
            lightMagenta,
            lightBlue,
            lightCyan,
            white,

            backDefault,
            backRed,
            backGreen,
            backYellow,
            backBlue
    };
}))
