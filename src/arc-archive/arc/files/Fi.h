//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_FI_H
#define MINDUSTRY_FI_H

#include <string>
#include <filesystem>
#include <memory>
#include <fstream>
#include <string>
#include <vector>

#include <Utilities.h>
#include <util/io/Writes.h>
#include <util/io/Reads.h>

enum class FileType;

class Fi {
protected:
    JFile file_;
    FileType type_;

    Fi() = default;

    explicit Fi(const std::string& fileName);

    explicit Fi(JFile file);

    Fi(const std::string& fileName, FileType type);

    Fi(JFile file, FileType type);

public:
    static std::shared_ptr<Fi> get(const std::string& path);

    static std::shared_ptr<Fi> tempFile(const std::string& prefix);

    static std::shared_ptr<Fi> tempDirectory(const std::string& prefix);

private:
    static void emptyDirectory(JFile file, bool preserveTree);

    static bool deleteDirectory(JFile file);

    static void copyFile(std::shared_ptr<Fi> source, std::shared_ptr<Fi> dest);

    static void copyDirectory(std::shared_ptr<Fi> sourceDir, std::shared_ptr<Fi> destDir);

public:
    std::string path() const;

    std::string absolutePath() const;

    std::string name() const;

    bool extEquals(const std::string& ext) const;

    std::string extension() const;

    std::string nameWithoutExtension() const;

    std::string pathWithoutExtension() const;

    FileType type() const;

    JFile file() const;

    std::ifstream read();

    std::ifstream read(std::size_t bufferSize);

    std::ifstream reader();

    std::ifstream reader(const std::string& charset);

    std::ifstream reader(std::size_t bufferSize);

    std::ifstream reader(std::size_t bufferSize, const std::string& charset);

    std::string readString();

    std::string readString(std::string charset);

    std::vector<std::uint8_t> readBytes();

    std::vector<std::uint8_t> readByteStream();

    std::size_t estimateLength();

    std::size_t readBytes(std::vector<std::uint8_t>& bytes, std::size_t offset, std::size_t size);

    std::vector<std::uint8_t> map();

    std::vector<std::uint8_t> map(java::nio::channels::FileChannel::MapMode mode);

    std::shared_ptr<Writes> writes(bool append);

    std::shared_ptr<Writes> writes();

    std::shared_ptr<Reads> reads();

    std::shared_ptr<Writes> writesDeflate();

    std::shared_ptr<Reads> readsDeflate();

    std::ofstream write();

    std::ofstream write(bool append);

    std::ofstream write(bool append, std::size_t bufsize);

    void write(std::ifstream input, bool append);

    std::ofstream writer(bool append);

    std::ofstream writer(bool append, const std::string& charset);
};

#endif //MINDUSTRY_FI_H
