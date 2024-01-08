//
// Created by teapot on 05/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTIL_OS_H
#define MINDUSTRY_SRC_ARC_UTIL_OS_H

#include <cstdint>
#include <memory>
#include <string>

#include <arc/Utils.h>

// Forward decl to avoid circular reference
class Fi;

class OS {
public:
    static std::uint32_t cores;
    static std::string username, userHome, osName, osVersion, osArch, osArchBits;
    static bool isWindows, isLinux, isMac, isIos, isAndroid, isARM, is64Bit;

private:
    sblockfdcl()

public:
    static std::string getAppDataDirectoryString(const std::string& appname);
    static std::string getWindowsTmpDir();

    static std::string exec(bool logErr, std::initializer_list<std::string> args);
    static std::string exec(std::initializer_list<std::string> args);

    static bool execSafe(std::initializer_list<std::string> cmd);

    static std::shared_ptr<Fi> getAppDataDirectory(const std::string& appname);

    static bool hasEnv(const std::string& name);
    static std::string env(const std::string& name);
};


#endif //MINDUSTRY_SRC_ARC_UTIL_OS_H
