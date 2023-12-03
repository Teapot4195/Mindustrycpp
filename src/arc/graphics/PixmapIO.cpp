//
// Created by teapot on 01/12/23.
//

#include "PixmapIO.h"

#include <arc/files/Fi.h>
#include <arc/graphics/Color.h>
#include <arc/graphics/Pixmap.h>
#include <arc/util/Exception.h>
#include <arc/util/io/WritesDeflate.h>
#include <arc/util/io/ReadsDeflate.h>
#include <arc/util/Exception.h>
#include <arc/util/Pack.h>

void PixmapIO::writeApix(const std::shared_ptr<Fi>& file, const std::shared_ptr<Pixmap>& pixmap) {
    try {
        std::shared_ptr<WritesDeflate> out = std::make_shared<WritesDeflate>(file->write(false));
        out->i(pixmap->width);
        out->i(pixmap->height);

        out->write(pixmap->pixels.data(), pixmap->pixels.size());
    } catch (const std::exception& e) {
        std::throw_with_nested(ArcRuntimeException("Couldn't write Pixmap to file '" + file->toString() + "'"));
    }
}

std::shared_ptr<Pixmap> PixmapIO::readApix(const std::shared_ptr<Fi>& file) {
    try {
        std::shared_ptr<ReadsDeflate> in = std::make_shared<ReadsDeflate>(file->read());
        std::shared_ptr<Pixmap> pixmap = std::make_shared<Pixmap>(in->i(), in->i());

        pixmap->pixels = in->read(pixmap->pixels.capacity());
        return pixmap;
    } catch (const std::exception& e) {
        std::throw_with_nested(ArcRuntimeException("Couldn't write Pixmap to file '" + file->toString() + "'"));
    }
}

void PixmapIO::writePng(const std::shared_ptr<Fi>& file, const std::shared_ptr<Pixmap>& pixmap) {
    try {
        PngWriter writer = PngWriter((int)((float)pixmap->width * (float)pixmap->height * 1.5f));
        writer.setFlipY(false);
        writer.write(file, pixmap);
    } catch (const std::exception& ex) {
        std::throw_with_nested(ArcRuntimeException("Error writing PNG: " + file->toString()));
    }
}

std::shared_ptr<Pixmap> PixmapIO::readPNG(std::shared_ptr<Fi> file) {
    try {
        PngReader reader = PngReader();
        std::vector<std::uint8_t> result = reader.read(file->read());
        return std::make_shared<Pixmap>(result, reader.width, reader.height);
    } catch (const std::exception& e) {
        std::throw_with_nested(ArcRuntimeException("Error reading PNG: " + file->toString()));
    }
}

PngChunkWriter::PngChunkWriter(std::size_t bufsize) : bufsize(bufsize) {
    data.reserve(bufsize);
}

void PngChunkWriter::endChunk(std::ofstream& stream) {
    auto sz = (int)(data.size() - 4);
    std::vector<char> szvec(reinterpret_cast<char*>(&sz), reinterpret_cast<char*>(&sz) + 4);
    stream.write(szvec.data(), 4);

    stream.write(reinterpret_cast<const char *>(data.data()), (long long)data.size());

    boost::crc_32_type accumulator;
    accumulator.process_bytes(data.data(), data.size());
    auto chk = accumulator.checksum();
    std::vector<char> chkvec(reinterpret_cast<char*>(&chk), reinterpret_cast<char*>(&chk) + 4);
    stream.write(chkvec.data(), 4);

    data.clear();
    data.reserve(bufsize);
}

void PngChunkWriter::wbyte(std::uint8_t byte) {
    data.push_back(byte);
    available++;
}

void PngChunkWriter::wint(std::uint32_t value) {
    std::vector<char> datavec(reinterpret_cast<char*>(&value), reinterpret_cast<char*>(&value) + 4);
    for (std::size_t i = 0; i < 4; i++) {
        wbyte(datavec[i]);
    }
}

void PngChunkWriter::warr(const std::vector<std::uint8_t>& vec) {
    for (const std::uint8_t &val: vec)
        wbyte(val);
}

void PngChunkWriter::warr(std::uint8_t *vec, std::size_t siz) {
    for (std::size_t i = 0; i < siz; i++)
        wbyte(vec[i]);
}

void PngChunkWriter::warr(std::uint8_t *beg, const std::uint8_t *end) {
    while (beg != end) {
        wbyte(*beg);
        beg++;
    }
}

PngWriter::PngWriter() : PngWriter(128 * 128) {}

PngWriter::PngWriter(std::size_t bufSize) {
    buffer = new PngChunkWriter(bufSize);
}

void PngWriter::setFlipY(bool flip) {
    flipY = flip;
}

[[maybe_unused]] void PngWriter::setCompression(int level) {
    throw ArcNotImplementedException("setCompression is not implemented!");
}

void PngWriter::write(const std::shared_ptr<Fi>& file, const std::shared_ptr<Pixmap>& pixmap) {
    std::ofstream output = file->write(false);
    write(output, pixmap);
}

void PngWriter::write(std::ofstream &output, const std::shared_ptr<Pixmap>& pixmap) {
    output.write(reinterpret_cast<const char *>(SIGNATURE), sizeof(SIGNATURE) / sizeof(SIGNATURE[0]));

    buffer->wint(IHDR);
    buffer->wint(pixmap->width);
    buffer->wint(pixmap->height);
    buffer->wbyte(8);
    buffer->wbyte(COLOR_ARGB);
    buffer->wbyte(COMPRESSION_DEFLATE);
    buffer->wbyte(FILTER_NONE);
    buffer->wbyte(INTERLACE_NONE);
    buffer->endChunk(output);

    buffer->wint(IDAT);
    deflater.reset();

    int lineLen = pixmap->width * 4;

    std::vector<std::uint8_t> curLine;
    auto* vec = new std::uint8_t[lineLen*2];

    for (int y = 0, h = pixmap->height; y < h; y++) {
        int py = flipY ? (h - y - 1) : y;
        auto start = pixmap->pixels.begin();

        curLine = std::vector<std::uint8_t>(start + py * lineLen, start + (py + 1) * lineLen);

        params.next_out = vec;
        params.next_in = curLine.data();
        params.avail_in = curLine.size();
        params.avail_out = lineLen * 2;

        boost::system::error_code e;

        deflater.write(params, boost::beast::zlib::Flush::none, e);

        // TODO: Perform Error Handling!

        buffer->warr(vec, (std::uint8_t*)params.next_out);
    }

    params.next_out = vec;
    params.next_in = curLine.data();
    params.avail_in = curLine.size();
    params.avail_out = lineLen * 2;

    boost::system::error_code e;

    deflater.write(params, boost::beast::zlib::Flush::none, e);

    // TODO: Perform Error Handling!

    buffer->warr(vec, (std::uint8_t*)params.next_out);

    buffer->endChunk(output);

    buffer->wint(IEND);
    buffer->endChunk(output);

    output.flush();
}

std::vector<std::uint8_t> PngReader::read(std::basic_ifstream<char> in) {
    readChunks(in);

    buf.clear();
    buf.resize(cs);
    try {
        getImage(buf);
    } catch (const std::exception &e) {
        std::throw_with_nested(IOException(""));
    }
    return buf;
}

void PngReader::readChunks(std::ifstream &in) {
    auto start = in.tellg();
    in.seekg(0, std::ios::end);
    if (imgData.empty() && in.tellg() - start > 4) {
        in.seekg(start);
        auto header = util::readIn<std::uint64_t>(in);
        if (header != 0x89504e470d0a1a0aULL) {
            std::string headerString = util::int_to_hex(header);
            throw IOException(headerString.starts_with("ffd8ff") ?
                    "This is a JPEG, not a PNG." :
                    "This isn't a PNG. Header: 0x" + headerString);
        }
    } else if (imgData.empty()) {
        width = 0;
        return;
    }
    dataLen = 0;
    std::uint32_t chunkType;
    while(true) {
        auto chunkLen = util::readIn<std::uint32_t>(in);
        if (chunkLen == 0 || chunkLen > 99998192)
            break;

        chunkType = util::readIn<std::uint32_t>(in);
        if (chunkType == 0x49454e44) //IEND
            break;
        if(chunkType == 0x49444154) { //IDAT
            in.read(reinterpret_cast<char *>(imgData.data() + dataLen), chunkLen);
            dataLen += chunkLen;
        } else if (chunkType == 0x49484452) { //IHDR
            width = util::readIn<std::uint32_t>(in);
            height = util::readIn<std::uint32_t>(in);
            bitDepth = util::readIn<std::uint8_t>(in);
            colorType = util::readIn<std::uint8_t>(in);
            compression = util::readIn<std::uint8_t>(in);
            filter = util::readIn<std::uint8_t>(in);
            interlace = util::readIn<std::uint8_t>(in);

            cs = 4 * width * height;
            start = in.tellg(); //Find where data started
            in.seekg(0, std::ios::end); // Seek to end
            imgData.clear();
            imgData.resize(in.tellg() - start); // use 'available count' to resize
            in.seekg(start); //Restore head

            //validation
            if (bitDepth == 16)
                throw IOException("16-bit depth is not supported.");
            if(colorType == ctypePalette && bitDepth < 4)
                throw IOException("Only PNG palettes with 4 or 8-bit depth are supported. Depth given: " + std::to_string(bitDepth));
            if(colorType != ctypePalette && colorType != ctypeRgb && colorType != ctypeRgba)
                throw IOException("Unsupported color type: " + std::to_string(colorType) + " (Note that grayscale is not supported)");
            if(interlace != 0)
                throw IOException("PNG interlacing is not supported.");
        } else if (colorType == ctypePalette && chunkType == 0x504c5445) { //PLTE
            int colors = (int)chunkLen / 3;
            palette.clear();
            palette.resize(colors);
            for(int i = 0; i < colors; i++)
                palette[i] = Color::packRgba(util::readIn<std::uint8_t>(in), util::readIn<std::uint8_t>(in), util::readIn<std::uint8_t>(in), 255);
        } else if (colorType == ctypePalette && chunkType == 0x74524e53) // tRNS
            for (int i = 0; i < chunkLen; i++)
                palette[i] = (palette[i] & 0xffffff00) | util::readIn<std::uint8_t>(in);
        else {
            std::vector<std::uint8_t> chunkData(chunkLen);
            in.read(reinterpret_cast<char *>(chunkData.data()), chunkLen);
        }
        util::readIn<std::uint32_t>(in);
    }
}

void PngReader::getImage(std::vector<std::uint8_t>& bb) {
    //bpx bytes per pixel, wT total output width, v scanline width
    std::uint32_t
            bpx = colorType == ctypePalette ? 1 : colorType == ctypeRgb ? 3 : 4,
            wT = width * 4,
            v = (bitDepth == 4 ? width / 2 : width) * bpx + 1; // scanLine width

    boost::beast::zlib::inflate_stream inflater;
    boost::beast::zlib::z_params params;
    boost::system::error_code err;

    // Inputs are always the same, we won't ever need to update this.
    params.next_in = imgData.data();
    params.avail_in = imgData.size();

    std::vector<std::uint8_t> prev(wT + 1), row(wT + 1);

    for (std::size_t i = 1, s = 0; s < height; i = 1, s++) {
        params.next_out = row.data();
        params.avail_out = row.size();

        inflater.write(params, boost::beast::zlib::Flush::none, err);

        // TODO: Perform Error Handling!

        auto first = row[0];

        if (first != 0) { // apply filters
            switch(first) {
                case 1:
                    for (i += bpx; i < v; i++)
                        row[i] += row[i - bpx];
                    break;
                case 2:
                    for (; i < v; i++)
                        row[i] += prev[i];
                    break;
                case 3:
                    for (; i < bpx + 1; i++)
                        row[i] += (prev[i] & 0xFF) >> 1;
                    for (; i < v; i++)
                        row[i] += ((prev[i] & 0xFF) + (row[i - bpx] & 0xFF)) >> 1;
                    break;
                default:
                    for (; i < bpx + 1; i++)
                        row[i] += prev[i];
                    for (; i < v; i++)
                        row[i] += paeth(row[i - bpx] & 0xFF, prev[i] & 0xFF, prev[i - bpx] & 0xFF);
                    break;
            }
        }

        // format output, normal mode
        if (bpx == 3) {
            for(i = 1; i < v; i+= bpx) {
                bb.push_back((std::uint8_t)(row[i] << 24 & 0xFF000000));
                bb.push_back((std::uint8_t)(row[i+1] << 16 & 0xFF0000));
                bb.push_back((std::uint8_t)(row[i+2] << 8 & 0xFF00));
                bb.push_back(0xFF);
            }
        } else if (bpx == 1) { // palette
            //when bitDepth is 4, split every byte in two
            if (bitDepth == 4)
                for (i = 0; i < v; i+= bpx) {
                    std::vector<std::uint8_t> obj = Pack::bytes((int)palette[Pack::leftByte(row[i])]);
                    bb.insert(bb.end(), obj.begin(), obj.end());
                    obj = Pack::bytes((int)palette[Pack::rightByte(row[i])]);
                    bb.insert(bb.end(), obj.begin(), obj.end());
                }
            else
                for(i = 1; i < v; i += bpx) {
                    std::vector<std::uint8_t> obj = Pack::bytes((int)palette[row[i] & 0xFF]);
                    bb.insert(bb.end(), obj.begin(), obj.end());
                }
        } else
            bb.insert(bb.end(), row.begin() + 1, row.end());

        std::swap(prev, row);
    }
    imgData.clear();
}

int PngReader::ab(int a) {
    int b = a >> 8;
    return (a ^ b) - b;
}

int PngReader::paeth(int a, int b, int c) {
    int pa = b - c, pb = a - c, pc = ab(pa + pb);
    pa = ab(pa);
    pb = ab(pb);
    return (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
}
