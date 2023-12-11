//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTILITIES_H
#define MINDUSTRY_SRC_ARC_UTILITIES_H

#include <filesystem>
#include <fstream>
#include <utility>

class Stringable {
public:
    virtual ~Stringable() = default;

    virtual std::string toString() = 0;
};

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

    class NamedTree {
    public:
        std::string type;

        NamedTree() : NamedTree("object") {}

        explicit NamedTree(std::string name) : type(std::move(name)) {}

        [[nodiscard]] std::string getInstof() const {
            return type;
        }
    };
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

    template< typename T >
    std::string int_to_hex( T i )
    {
        std::stringstream stream;
        stream << "0x"
               << std::setfill ('0') << std::setw(sizeof(T)*2)
               << std::hex << i;
        return stream.str();
    }

    template <typename T>
    T readIn(std::istream& in) {
        char a[sizeof(T)];
        in.read(a, sizeof(T));
        T result;
        for (char &e : a ) {
            result <<= 8;
            result &= ~T(0) ^ 0xFF; //Generate a bitmask of the form 0xFF00 with as many FF on the end as size.
            result |= e;
        }

        return result;
    }

    std::string& replaceAll(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
}

#endif //MINDUSTRY_SRC_ARC_UTILITIES_H
