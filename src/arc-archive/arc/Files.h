//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_FILES_H
#define MINDUSTRY_FILES_H

#include <string>
#include <utility>
#include <memory>
#include <unordered_map>

#include <files/Fi.h>
#include <Utilities.h>

enum class FileType {
    classpath,
    internal,
    external,
    absolute,
    local
};

struct FileType_ {
    static const std::unordered_map<std::string, FileType> StrToFileType;
    static const std::unordered_map<FileType, std::string> FileTypeToStr;

    static FileType get(const std::string& str) {
        if (StrToFileType.contains(str)) {
            return StrToFileType.at(str);
        } else {
            throw util::exc::IndexOutOfBounds("Str to FileType got unknown str \'" + str + "\'");
        }
    }

    static std::string str(FileType type) {
        if (FileTypeToStr.contains(type)) {
            return FileTypeToStr.at(type);
        } else {
            throw util::exc::IndexOutOfBounds("FileType to str got unknown FileType \'" + std::to_string((int)type) + "\'");
        }
    }
};

const std::unordered_map<FileType, std::string> FileType_::FileTypeToStr {
        {FileType::classpath, "classpath"},
        {FileType::internal, "internal"},
        {FileType::external, "external"},
        {FileType::absolute, "absolute"},
        {FileType::local, "local"}
};

const std::unordered_map<std::string, FileType> FileType_::StrToFileType {
        {"classpath", FileType::classpath},
        {"internal", FileType::internal},
        {"external", FileType::external},
        {"absolute", FileType::absolute},
        {"local", FileType::local}
};

class Files {
public:
    virtual ~Files() = default;

    virtual std::shared_ptr<Fi> get(std::string path, FileType type) = 0;

    virtual std::shared_ptr<Fi> classpath(std::string path) {
        return get(std::move(path), FileType::classpath);
    }

    virtual std::shared_ptr<Fi> internal(std::string path) {
        return get(std::move(path), FileType::internal);
    }

    virtual std::shared_ptr<Fi> external(std::string path) {
        return get(std::move(path), FileType::external);
    }

    virtual std::shared_ptr<Fi> absolute(std::string path) {
        return get(std::move(path), FileType::absolute);
    }

    virtual std::shared_ptr<Fi> local(std::string path) {
        return get(std::move(path), FileType::local);
    }

    virtual std::shared_ptr<Fi> cache(std::string path) {
        return get(getCachePath(), FileType::absolute)->child(path);
    }

    virtual std::string getCachePath() {
        return local("cache")->absolutePath();
    }

    virtual std::string getExternalStoragePath() = 0;

    virtual bool isExternalStorageAvailable() = 0;

    virtual std::string getLocalStoragePath() = 0;

    virtual bool isLocalStorageAvailable() = 0;
};

#endif //MINDUSTRY_FILES_H
