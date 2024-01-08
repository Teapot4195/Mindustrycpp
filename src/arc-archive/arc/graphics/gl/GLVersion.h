//
// Created by teapot on 04/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_GL_GLVERSION_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_GL_GLVERSION_H

#include <string>
#include <unordered_map>

#include <arc/Utilities.h>

// Forward decls to avoid circular reference
enum class ApplicationType;

class GLVersion : public Stringable {
public:
    enum GlType {
        OpenGL,
        GLES,
        WebGL,
        NONE
    };

    const std::unordered_map<GlType, std::string> convertor {
            {OpenGL, "OpenGL"},
            {GLES, "GLES"},
            {WebGL, "WebGL"},
            {NONE, "NONE"}
    };

    std::string vendorString;
    std::string rendererString;
    GlType type;
    int majorVersion, minorVersion, releaseVersion{};

    GLVersion(ApplicationType appType, const std::string& versionString, std::string vendorString1, std::string rendererString1);

private:
    void extractVersion(const std::string& patternString, const std::string& versionString);

    static int parseInt(const std::string& v, int defaultValue);

public:
    [[nodiscard]] bool atLeast(int testMajorVersion, int testMinorVersion) const;

    [[nodiscard]] std::string getDebugVersionString() const;

    [[nodiscard]] std::string toString() const;
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_GL_GLVERSION_H
