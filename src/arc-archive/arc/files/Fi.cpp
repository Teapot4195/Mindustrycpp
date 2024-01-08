//
// Created by teapot on 20/11/23.
//

#include "Fi.h"
#include "util/io/WritesDeflate.h"

#include <utility>
#include <cstdio>
#include <vector>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <Files.h>
#include <Utilities.h>
#include <Core.h>
#include <Constants.h>
#include <util/Exception.h>

Fi::Fi(const std::string& fileName) : file_(fileName), type_(FileType::absolute) {}

Fi::Fi(JFile file) : file_(std::move(file)), type_(FileType::absolute) {}

Fi::Fi(const std::string& fileName, FileType type) : file_(fileName), type_(type) {}

Fi::Fi(JFile file, FileType type) : file_(std::move(file)), type_(type) {}

std::shared_ptr<Fi> Fi::get(const std::string& path) {
    return std::shared_ptr<Fi>(new Fi(path));
}

std::shared_ptr<Fi> Fi::tempFile(const std::string& prefix) {
    try {
        return std::shared_ptr<Fi>(new Fi(util::MakeTemporaryFile(prefix, "")));
    } catch (const std::runtime_error& ex) {
        std::throw_with_nested(ArcRuntimeException("Unable to create temp file_."));
    }
}

std::shared_ptr<Fi> Fi::tempDirectory(const std::string &prefix) {
    try {
        return std::shared_ptr<Fi>(new Fi(util::MakeTemporaryFolder(prefix)));
    } catch (const std::runtime_error& ex) {
        std::throw_with_nested(ArcRuntimeException("Unable to create temp folder."));
    }
}

void Fi::emptyDirectory(JFile file, bool preserveTree) {
    if (exists(file.path)) {
        std::filesystem::recursive_directory_iterator files {file.path};
        for(std::filesystem::path value : files) {
            if (!is_directory(value))
                remove(value);
            else if (preserveTree)
                emptyDirectory({value}, true);
            else
                deleteDirectory({value});
        }
    }
}

bool Fi::deleteDirectory(JFile file) {
    return remove(file.path);
}

void Fi::copyFile(std::shared_ptr<Fi> source, std::shared_ptr<Fi> dest) {
    try {
        dest.write(source.read, false);
    } catch (const std::exception& ex) {
        std::throw_with_nested(ArcRuntimeException(
                "Error copying source file_: " + source->file_.path.string() + " (" + FileType_::str(source->type_) + ")\n"
                "To destination: " + dest->file_.path.string() + " (" + FileType_::str(dest->type_) + ")"
        ));
    }
}

void Fi::copyDirectory(std::shared_ptr<Fi> sourceDir, std::shared_ptr<Fi> destDir) {
    destDir.mkdirs();
    std::vector<std::shared_ptr<Fi>> files = sourceDir.list();
    for (std::shared_ptr<Fi> srcFile: files) {
        std::shared_ptr<Fi> destFile = destDir.child(srcFile->name());
        if (srcFile.isDirectory())
            copyDirectory(srcFile, destFile);
        else
            copyFile(srcFile, destFile);
    }
}

std::string Fi::path() const {
    std::string f = file_.path.string();
    std::replace(f.begin(), f.end(), '\\', '/');
    return f;
}

std::string Fi::absolutePath() const {
    std::string f = absolute(file_.path);
    std::replace(f.begin(), f.end(), '\\', '/');
    return f;
}

std::string Fi::name() const {
    return file_.path.filename().string().empty() ? file_.path.string() : file_.path.filename().string();
}

bool Fi::extEquals(const std::string& ext) const {
    return boost::iequals(extension(), ext);
}

std::string Fi::extension() const {
    std::string name = file_.path.filename();
    auto dotIndex = name.find_last_of('.');
    if (dotIndex == name.length())
        return "";
    return name.substr(dotIndex + 1);
}

std::string Fi::nameWithoutExtension() const {
    std::string name = file_.path.filename();
    auto dotIndex = name.find_last_of('.');
    if (dotIndex == name.length())
        return name;
    return name.substr(0, dotIndex);
}

std::string Fi::pathWithoutExtension() const {
    std::string name = file_.path.string();
    std::replace(name.begin(), name.end(), '\\', '/');
    auto dotIndex = name.find_last_of('.');
    if (dotIndex == name.length())
        return name;
    return name.substr(0, dotIndex);
}

FileType Fi::type() const {
    return type_;
}

JFile Fi::file() const {
    if (type_ == FileType::external)
        return {Core::files->getExternalStoragePath() / file_.path};
    return file_;
}

std::ifstream Fi::read() {
    if (type_ == FileType::classpath || (type_ == FileType::internal && !exists(file().path))
    || (type_ == FileType::local && !exists(file().path))) {
        std::string path = "/" + file_.path.string();
        std::replace(path.begin(), path.end(), '\\', '/');
        std::ifstream input = std::ifstream("/" + path);
        if (!input) {
            throw ArcRuntimeException("File not found: " + file_.path.string() + "(" + FileType_::str(type_) + ")");
        }
        return input;
    }
    try {
        return {file().path};
    } catch (const std::exception& ex) {
        if (is_directory(file().path))
            std::throw_with_nested(ArcRuntimeException(
                    "Cannot open a stream to a directory: " + file_.path.string() + " (" + FileType_::str(type_) + ")"
            ));
        std::throw_with_nested(ArcRuntimeException(
                "Error reading file: " + file_.path.string() + " (" + FileType_::str(type_) + ")"
        ));
    }
}

std::ifstream Fi::read(std::size_t bufferSize) {
    return read();
}

std::ifstream Fi::reader() {
    return read();
}

std::ifstream Fi::reader(const std::string& charset) {
    return read();
}

std::ifstream Fi::reader(std::size_t bufferSize) {
    return read();
}

std::ifstream Fi::reader(std::size_t bufferSize, const std::string &charset) {
    return read();
}

std::string Fi::readString() {
    return readString("UTF-8");
}

std::string Fi::readString(std::string charset) {
    std::string output;
    try {
        std::ifstream reader = read();
        char buffer[257];
        std::size_t count;
        while((count = reader.readsome(buffer, 256)) && count != 0) {
            buffer[count] = '\0';
            output.append(buffer);
        }
    } catch (const std::exception& ex) {
        std::throw_with_nested(ArcRuntimeException("Error reading layout file: " + this->toString()));
    }

    return output;
}

std::vector<std::uint8_t> Fi::readBytes() {
    std::ifstream input = read();
    try {
        std::vector<std::uint8_t> result;
        char buffer[256];
        std::size_t count;
        while((count = input.readsome(buffer, 256)) && count != 0) {
            result.insert(result.end(), buffer, buffer + count);
        }
        return result;
    } catch (const std::exception& ex) {
        std::throw_with_nested(ArcRuntimeException("Error reading file: " + this->toString()));
    }
}

std::vector<std::uint8_t> Fi::readByteStream() {
    //TODO: In the future, return an actual custom byte stream
    return readBytes();
}

std::size_t Fi::estimateLength() {
    std::size_t length = length();
    return length != 0 ? length : 512;
}

std::size_t Fi::readBytes(std::vector<std::uint8_t> &bytes, std::size_t offset, std::size_t size) {
    std::ifstream input = read();
    std::size_t position = 0;
    try {
        input.ignore((std::streamsize)offset);
        position = input.readsome((char*)bytes.data(), (std::streamsize)size);
        return position;
    } catch (const std::exception& ex) {
        std::throw_with_nested(ArcRuntimeException("Error reading file: " + this->toString()));
    }
}

std::vector<std::uint8_t> Fi::map() {
    return map(java::nio::channels::FileChannel::MapMode::READ_ONLY);
}

std::vector<std::uint8_t> Fi::map(java::nio::channels::FileChannel::MapMode mode) {
    using namespace java::nio::channels;
    if (type_ == FileType::classpath)
        throw ArcRuntimeException("Cannot map a classpath file: " + this->toString());

    std::ifstream raf;
    std::vector<std::uint8_t> result;

    try {
        raf = std::ifstream(file_.path);
        char buffer[257];
        std::size_t count = 0;
        while ((count = raf.readsome(buffer, 256)) && count != 0) {
            buffer[count] = '\0';
            result.insert(result.end(), buffer, buffer + count);
        }
    } catch (const std::exception& ex) {
        std::throw_with_nested(ArcRuntimeException("Error memory mapping file: " + this->toString() + "(" + FileType_::str(type_) + ")"));
    }
}

std::shared_ptr<Writes> Fi::writes(bool append) {
    return std::make_shared<Writes>(std::ofstream(write(append, STREAMS_DEFAULTBUFFERSIZE)));
}

std::shared_ptr<Writes> Fi::writes() {
    return writes(false);
}

std::shared_ptr<Reads> Fi::reads() {
    return std::make_shared<Reads>(std::ifstream(read(STREAMS_DEFAULTBUFFERSIZE)));
}

std::shared_ptr<Writes> Fi::writesDeflate() {
    return std::make_shared<WritesDeflate>(writes());
}

std::shared_ptr<Reads> Fi::readsDeflate() {
    //TODO: Reads deflate should deflate.
    return reads();
}

std::ofstream Fi::write() {
    return write(false);
}

std::ofstream Fi::write(bool append) {
    using namespace std::string_literals;
    if (type_ == FileType::classpath || type_ == FileType::internal)
        throw ArcRuntimeException("Cannot write to "s + (type_ == FileType::classpath ? "a classpath" : "an internal") + " file: " + file_.path.string());
    parent().mkdirs();
    try {
        if (append)
            return {file().path, std::ios::app};
        else
            return {file().path};
    } catch (const std::exception& e) {
        if (is_directory(file().path))
            std::throw_with_nested(ArcRuntimeException("Cannot open a stream to a directory: " + file_.path.string() + " (" + FileType_::str(type_) + ")"));
        std::throw_with_nested(ArcRuntimeException("Error writing file: "s + file_.path.string() + " (" + FileType_::str(type_) + ")"));
    }
}

std::ofstream Fi::write(bool append, std::size_t bufsize) {
    return write(append);
}

void Fi::write(std::ifstream input, bool append) {
    try {
        std::ofstream output = write(append);
        while (!input.eof()) {
            output.put((char)input.get());
        }
    } catch (const std::exception& e) {
        std::throw_with_nested(ArcRuntimeException("Error writing to file: " + file_.path.string() + " (" + FileType_::str(type_) + ")"));
    }
}

std::ofstream Fi::writer(bool append) {
    return writer(append, "UTF-8");
}

std::ofstream Fi::writer(bool append, const std::string &charset) {
    if (type_ == FileType::classpath)
        throw ArcRuntimeException("Cannot write to a classpath file: " + file_.path.string());
    if (type_ == FileType::internal)
        throw ArcRuntimeException("Cannot write to an internal file:" + file_.path.string());

    parent().mkdirs();

    try {
        std::ofstream output;
        if (append)
            output = std::ofstream(file().path, std::ios::app);
        else
            output = std::ofstream(file().path);
        return output;
    } catch (const std::exception& e) {
        if (is_directory(file().path))
            std::throw_with_nested(ArcRuntimeException("Cannot open a stream to a directory: " + file_.path.string() + " (" + FileType_::str(type_) + ")"));
        std::throw_with_nested(ArcRuntimeException("Error writing file: " + file_.path.string() + " (" + FileType_::str(type_) + ")"));
    }
}
