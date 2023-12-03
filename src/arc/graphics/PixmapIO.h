//
// Created by teapot on 01/12/23.
//

#ifndef MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAPIO_H
#define MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAPIO_H

#include <memory>
#include <cstdint>

#include <boost/crc.hpp>
#include <boost/beast/zlib/deflate_stream.hpp>

#include <arc/util/Disposable.h>

// Forward decls to avoid circular reference
class Fi;
class Pixmap;

class PngChunkWriter {
private:
    std::vector<std::uint8_t> data;
    std::size_t available{0}, bufsize{0};

public:
    explicit PngChunkWriter(std::size_t bufsize);

    void endChunk(std::ofstream& stream);

    void wbyte(std::uint8_t byte);

    void wint(std::uint32_t value);

    void warr(const std::vector<std::uint8_t>& vec);

    void warr(std::uint8_t *vec, std::size_t siz);

    void warr(std::uint8_t *beg, const std::uint8_t* end);
};

class PngWriter /* Does not need disposing! : public Disposable*/ {
private:
    constexpr static const std::uint8_t SIGNATURE[] =  {137, 80, 78, 71, 13, 10, 26, 10};
    constexpr static const int IHDR = 0x49484452, IDAT = 0x49444154, IEND = 0x49454E44;
    constexpr static const std::uint8_t COLOR_ARGB = 6;
    constexpr static const std::uint8_t COMPRESSION_DEFLATE = 0;
    constexpr static const std::uint8_t FILTER_NONE = 0;
    constexpr static const std::uint8_t INTERLACE_NONE = 0;

    PngChunkWriter* buffer{};
    boost::beast::zlib::deflate_stream deflater;
    boost::beast::zlib::z_params params;
    bool flipY{true};

public:
    PngWriter();

    explicit PngWriter(std::size_t bufSize);

    void setFlipY(bool flip);

    [[maybe_unused]] void setCompression(int level);

    void write(const std::shared_ptr<Fi>& file, const std::shared_ptr<Pixmap>& pixmap);

    void write(std::ofstream& output, const std::shared_ptr<Pixmap>& pixmap);
};

class PngReader {
private:
    constexpr static const std::uint32_t
            ctypeRgba = 6,
            ctypePalette = 3,
            ctypeRgb = 2;

public:
    std::uint32_t width, height;

    std::uint8_t bitDepth, colorType, compression, filter, interlace;

private:
    std::uint32_t dataLen, cs;
    std::vector<std::uint8_t> imgData;
    std::vector<std::uint8_t> buf;
    std::vector<std::uint32_t> palette;

public:
    std::vector<std::uint8_t> read(std::basic_ifstream<char> in);

    void readChunks(std::ifstream& in);

private:
    void getImage(std::vector<std::uint8_t>& bb);

    static int ab(int a);

    static int paeth(int a, int b, int c);
};

class PixmapIO {
public:
    static void writeApix(const std::shared_ptr<Fi>& file, const std::shared_ptr<Pixmap>& pixmap);

    static std::shared_ptr<Pixmap> readApix(const std::shared_ptr<Fi>& file);

    static void writePng(const std::shared_ptr<Fi>& file, const std::shared_ptr<Pixmap>& pixmap);

    static std::shared_ptr<Pixmap> readPNG(std::shared_ptr<Fi> file);
};


#endif //MINDUSTRY_SRC_ARC_GRAPHICS_PIXMAPIO_H
