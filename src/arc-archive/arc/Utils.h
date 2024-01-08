//
// Created by teapot on 28/11/23.
//
// Header only utils that can be included without making a possible mess

#ifndef MINDUSTRY_SRC_ARC_UTILS_H
#define MINDUSTRY_SRC_ARC_UTILS_H

// Convenient way to define a "Static block like those in java"
#define sblockfdcl() static bool sblock_;
#define sblock(def_loc, body) bool def_loc::sblock_ = []() -> bool {body; return true;}();

#endif //MINDUSTRY_SRC_ARC_UTILS_H
