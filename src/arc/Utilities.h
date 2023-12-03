//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTILITIES_H
#define MINDUSTRY_SRC_ARC_UTILITIES_H

#include <filesystem>
#include <fstream>
#include <utility>

struct JFile {
    JFile(const JFile &file) : path(file.path) {}
    JFile(std::filesystem::path path) : path(std::move(path)) {}
    JFile(std::filesystem::path path1, std::fstream fstream) : path(std::move(path1)), stream(std::move(fstream)) {}

    std::filesystem::path path;
    std::fstream stream;
};

namespace java {
    namespace nio {
        namespace channels {
            struct FileChannel {
                enum class MapMode {
                    PRIVATE,
                    READ_ONLY,
                    READ_WRITE
                };

                struct MapMode_ {

                };
            };
        }
    }
}

namespace util {
    namespace exc {
#define NewBasicRuntimeError(Name) struct Name : public std::runtime_error { using std::runtime_error::runtime_error; };
        /**
         * @brief Exception is thrown when a length precondition isn't met.
         *
         * For example, this exception is thrown when MakeTemporaryFile does not get a prefix longer than 3 chars.
         */
        NewBasicRuntimeError(LengthException)

        NewBasicRuntimeError(IndexOutOfBounds)
#undef NewBasicRuntimeError
    }

    JFile MakeTemporaryFile(const std::string &prefix, const std::string &suffix) {
        if (prefix.length() <= 3) {
            throw exc::LengthException("Prefix must be longer than 3 characters! got" + std::to_string(prefix.length()));
        }

        std::filesystem::path TmpPath = std::filesystem::temp_directory_path();
        TmpPath /= prefix;
        TmpPath += '.';
        TmpPath += suffix.empty() ? ".tmp" : suffix;
        return {TmpPath, std::fstream(TmpPath)};
    }

    JFile MakeTemporaryFolder(const std::string &prefix) {
        if (prefix.length() <= 3)
            throw exc::LengthException("Prefix must be longer than 3 characters! got" + std::to_string(prefix.length()));

        std::filesystem::path TmpPath = std::filesystem::temp_directory_path();
        TmpPath /= prefix;

        return {TmpPath};
    }
}

#endif //MINDUSTRY_SRC_ARC_UTILITIES_H
