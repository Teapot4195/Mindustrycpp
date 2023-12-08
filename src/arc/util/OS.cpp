//
// Created by teapot on 05/12/23.
//

#include "OS.h"

#include <future>
#include <thread>

#include <infoware/infoware.hpp>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>
#include <boost/predef.h>

#if defined(BOOST_OS_LINUX) || defined(BOOST_OS_MAC) || defined(BOOST_OS_UNIX)
#include <unistd.h>
#else
#include <windows.h>
#include <Lmcons.h>
#endif

#include <arc/Core.h>
#include <arc/Files.h>
#include <arc/util/Exception.h>

std::string userHome;

sblock(OS, {
    cores = iware::cpu::quantities().logical;
    auto osInfo = iware::system::OS_info();
    osName = osInfo.name;
    osVersion = osInfo.full_name;

#if defined(BOOST_OS_LINUX) || defined(BOOST_OS_MAC) || defined(BOOST_OS_UNIX)
    username = std::string(getlogin());
    userHome = getenv("HOME");
#else
    userHome = std::string(getenv("HOMEDRIVE")) + std::string(getenv("HOMEPATH"));
    char uanme[UNLEN+1];
    DWORD uname_len = UNLEN+1;
    GetUserName(uanme, &uname_len);
    username = std::string(uanme, uname_len);
#endif

#ifdef BOOST_OS_LINUX
    isLinux = true;
#else
    isLinux = false;
#endif
#ifdef BOOST_OS_WINDOWS
    isWindows = true;
#else
    isWindows = false;
#endif
#ifdef BOOST_OS_MACOS
    isMac = true;
#else
    isMac = true;
#endif
#ifdef BOOST_PLAT_IOS_DEVICE
    isIos = true;
#else
    isIos = false;
#endif
#ifdef BOOST_PLAT_ANDROID
    isAndroid = true;
#else
    isAndroid = false;
#endif
#ifdef BOOST_ARCH_ARM
    isARM = true;
#else
    isARM = false;
#endif
#ifdef BOOST_ARCH_WORD_BITS_64
    is64Bit = true;
#else
    is64Bit = false;
#endif

#ifdef BOOST_ARCH_ALPHA
    osArch = "DEC_Alpha";
#elifdef BOOST_ARCH_ARM
    osArch = "ARM";
#elifdef BOOST_ARCH_BLACKFIN
    osArch = "Blackfin";
#elifdef BOOST_ARCH_CONVEX
    osArch = "Convex";
#elifdef BOOST_ARCH_E2K
    osArch = "E2K";
#elifdef BOOST_ARCH_IA64
    osArch = "Itanium64";
#elifdef BOOST_ARCH_M68K
    osArch = "M68K";
#elifdef BOOST_ARCH_MIPS
    osArch = "MIPS";
#elifdef BOOST_ARCH_PARISC
    osArch = "PaRISC";
#elifdef BOOST_ARCH_PPC
    osArch = "PowerPC";
#elifdef BOOST_ARCH_PPC_64
    osArch = "PowerPC";
#elifdef BOOST_ARCH_PTX
    osArch = "PTX";
#elifdef BOOST_ARCH_PYRAMID
    osArch = "Pyramid9810";
#elifdef BOOST_ARCH_RISCV
    osArch = "RISC-V";
#elifdef BOOST_ARCH_RS6000
    osArch = "RS6000";
#elifdef BOOST_ARCH_SPARC
    osArch = "SPARC";
#elifdef BOOST_ARCH_SH
    osArch = "SuperH";
#elifdef BOOST_ARCH_SYS370
    osArch = "Sys370";
#elifdef BOOST_ARCH_SYS390
    osArch = "Sys390";
#elifdef BOOST_ARCH_X86
    osArch = "X86";
#elifdef BOOST_ARCH_Z
    osArch = "Z";
#endif

#ifdef BOOST_ARCH_WORD_BITS_16
    osArchBits = "16";
#elifdef BOOST_ARCH_WORD_BITS_32
    osArchBits = "32";
#elifdef BOOST_ARCH_WORD_BITS_64
    osArchBits = "64";
#endif
})

bool OS::hasEnv(const std::string& name) {
    return getenv(name.c_str()) != nullptr;
}

std::string OS::env(const std::string& name) {
    return {getenv(name.c_str())};
}

std::string OS::getAppDataDirectoryString(const std::string& appname) {
    if (isWindows) {
        return env("AppData") + "\\\\" + appname;
    } else if (isIos || isAndroid) {
        return Core::files.value()->getLocalStoragePath();
    } else if (isLinux) {
        if (hasEnv("XDG_DATA_HOME")) {
            std::string dir = env("XDG_DATA_HOME");
            if (!dir.ends_with("/"))
                dir += "/";
            return dir + appname + "/";
        }
        return userHome + "/.local/share/" + appname + "/";
    } else if (isMac) {
        return userHome + "/Library/Application Support/" + appname + "/";
    } else { // Probably web
        return "";
    }
}

std::string OS::getWindowsTmpDir() {
    if(hasEnv("TEMP"))
        return env("TEMP");
    if(hasEnv("TMP"))
        return env("TMP");
    return "C:\\Windows\\TEMP";
}

std::string OS::exec(bool logErr, std::initializer_list<std::string> args) {
    std::stringstream result;

    reproc::process proc;
    reproc::options opt;

    std::error_code ec = proc.start(args, opt);

    if (ec) {
        throw ArcRuntimeException("Error orccured when trying to start program");
    }

    auto drain = std::async(std::launch::async, [&]() {
        if (logErr) {
            reproc::sink::ostream stream(result);
            return reproc::drain(proc, stream, stream);
        } else {
            reproc::sink::discard err;
            reproc::sink::ostream out(result);
            return reproc::drain(proc, out, err);
        }
    });

    if (!drain.valid())
        throw std::future_error(std::future_errc::no_state);
    drain.wait();

    ec = drain.get();
    if (ec) {
        throw ArcRuntimeException("Error occurred when getting program output");
    }

    std::string res = result.str();

    if (!res.empty() && res[res.length() - 1] == '\n')
        return res.substr(0, res.length() - 1);

    return result.str();
}

std::string OS::exec(std::initializer_list<std::string> args) {
    return exec(false, args);
}

bool OS::execSafe(std::initializer_list<std::string> cmd) {
    reproc::process proc;

    std::error_code ec = proc.start(cmd);

    if (ec) {
        throw ArcRuntimeException("Error orccured when trying to start program");
    }

    auto drain = std::async(std::launch::async, [&]() {
        reproc::sink::ostream stream(std::cout);
        return reproc::drain(proc, stream, stream);
    });

    if (!drain.valid())
        throw std::future_error(std::future_errc::no_state);
    drain.wait();

    ec = drain.get();
    if (ec) {
        return false;
    }

    return true;
}

std::shared_ptr<Fi> OS::getAppDataDirectory(const std::string& appname) {
    return Core::files.value()->absolute(getAppDataDirectoryString(appname));
}
