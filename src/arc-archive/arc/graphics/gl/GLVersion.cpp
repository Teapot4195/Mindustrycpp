//
// Created by teapot on 04/12/23.
//

#include "GLVersion.h"

#include <utility>
#include <regex>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <arc/Application.h>
#include <arc/util/Log.h>

GLVersion::GLVersion(ApplicationType appType, const std::string& versionString, std::string vendorString1,
                     std::string rendererString1) {
    switch(appType) {
        case ApplicationType::android:
        case ApplicationType::iOS:
            type = GlType::GLES;
            //OpenGL<space>ES<space><version number><space><vendor-specific information>.
            extractVersion(R"(OpenGL ES (\d(\.\d){0,2}))", versionString);
            break;
        case ApplicationType::desktop:
            type = GlType::OpenGL;
            //WebGL<space><version number><space><vendor-specific information>
            extractVersion(R"(WebGL (\d(\.\d){0,2}))", versionString);
            break;
        case ApplicationType::web:
            type = GlType::WebGL;
            //<version number><space><vendor-specific information>
            extractVersion(R"((\d(\.\d){0,2}))", versionString);
            break;
        default:
            type = GlType::NONE;
            majorVersion = -1;
            minorVersion = -1;
            vendorString = "";
            rendererString = "";
            break;
    }

    vendorString = std::move(vendorString1);
    rendererString = std::move(rendererString1);
}

void GLVersion::extractVersion(const std::string& patternString, const std::string& versionString) {
    std::regex pattern(patternString);
    std::smatch results;
    std::regex_search(versionString, results, pattern);
    if (results.empty()) {
        std::string result = results[1].str();
        std::vector<std::string> resultSplit;
        boost::split(resultSplit, result, boost::is_any_of("."), boost::token_compress_on);
        majorVersion = parseInt(resultSplit[0], 2);
        minorVersion = resultSplit.size() < 2 ? 0 : parseInt(resultSplit[1], 0);
        releaseVersion = resultSplit.size() < 3 ? 0 : parseInt(resultSplit[2], 0);
    } else {
        Log::err("[Arc GL] Invalid version string: " + versionString);
        majorVersion = 2;
        minorVersion = 0;
        releaseVersion = 0;
    }
}

int GLVersion::parseInt(const std::string& v, int defaultValue) {
    try {
        return std::stoi(v);
    } catch (const std::invalid_argument& nfe) {
        Log::err("[Arc GL] Error parsing number: " + v + ", assuming: " + std::to_string(defaultValue));
        return defaultValue;
    }
}

bool GLVersion::atLeast(int testMajorVersion, int testMinorVersion) const {
    return majorVersion > testMajorVersion || (majorVersion == testMajorVersion && minorVersion >= testMinorVersion);
}

std::string GLVersion::getDebugVersionString() const {
    return "Type: " + convertor.at(type) + "\n" +
           "Version: " + std::to_string(majorVersion) + ":" + std::to_string(minorVersion) + ":" + std::to_string(releaseVersion) + "\n" +
           "Vendor: " + vendorString + "\n" +
           "Renderer: " + rendererString;
}

std::string GLVersion::toString() const {
    return convertor.at(type) + " " +
           std::to_string(majorVersion) + "." +
           std::to_string(minorVersion) + "." +
           std::to_string(releaseVersion) + " / " +
           vendorString + " / " +
           rendererString;
}
