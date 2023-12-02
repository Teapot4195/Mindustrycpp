
//
// Created by teapot on 01/12/23.
//

#include "WritesDeflate.h"

//TODO: Make these rethrow exception descriptions way more creative than "Exception was thrown."!

std::shared_ptr<WritesDeflate> WritesDeflate::instance = std::make_shared<WritesDeflate>();

WritesDeflate::WritesDeflate() = default;

WritesDeflate::WritesDeflate(std::ofstream output) : output(std::make_optional<std::ofstream>(std::move(output))) {}

std::shared_ptr<WritesDeflate> WritesDeflate::get(std::ofstream output) {
    instance->output = std::move(output);
    return instance;
}

WritesDeflate::WritesDeflate(const std::shared_ptr<Writes>& write) : output(std::move(write->output)) {}

void WritesDeflate::l(long long i) {
    try {
        std::uint8_t a[sizeof(long long)];
        for (std::uint8_t &f : a) {
            f = (char)(i & 0xFFl);
            i >>= 8;
        }
        write(a, sizeof(long long));
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void WritesDeflate::i(int i) {
    try {
        std::uint8_t a[sizeof(int)];
        for (std::uint8_t &f : a) {
            f = (char)(i & 0xFFl);
            i >>= 8;
        }
        write(a, sizeof(int));
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void WritesDeflate::b(int i) {
    try {
        write(i & 0xFF);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void WritesDeflate::b(std::vector<std::uint8_t> array, int offset, std::size_t length) {
    try {
        write(array.data(), length);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void WritesDeflate::b(const std::vector<std::uint8_t>& array) {
    b(array, 0, array.size());
}

void WritesDeflate::boolean(bool b) {
    this->b(b ? 1 : 0);
}

void WritesDeflate::s(int i) {
    try {
        std::uint8_t a[sizeof(short)];
        for (std::uint8_t &f : a) {
            f = (char)(i & 0xFFl);
            i >>= 8;
        }
        write(a, sizeof(short));
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void WritesDeflate::f(float f) {
    try {
        auto i = std::bit_cast<std::uint32_t>(f);
        std::uint8_t a[4];
        for (std::uint8_t &e : a) {
            e = (char)(i & 0xFFl);
            i >>= 8;
        }
        write(a, 4);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void WritesDeflate::d(double d) {
    try {
        auto i = std::bit_cast<std::uint64_t>(d);
        std::uint8_t a[8];
        for (std::uint8_t &e : a) {
            e = (char)(i & 0xFFl);
            i >>= 8;
        }
        write(a, 8);
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

void WritesDeflate::str(std::string str) {
    try {
        const char* buf = str.c_str();
        write((str.size() & 0xFF00) >> 8);
        write(str.size() & 0xFF);
        for (int i = 0; i < str.size(); i++) {
            write(0x00);
            write(buf[i]);
        }
    } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error("Exception was thrown."));
    }
}

WritesDeflate::~WritesDeflate() {
    if (output.has_value())
        try {
            output->close();
        } catch (const std::exception& e) {
            std::throw_with_nested(std::runtime_error("Exception was thrown."));
        }
}

void WritesDeflate::flush() {
    params.next_in = usebufa ? inbufa : inbufb;
    params.avail_in = avail_in;
    params.next_out = obuf;
    params.avail_out = buf_size;
    boost::system::error_code ecode{};
    stream.write(params, boost::beast::zlib::Flush::none, ecode);

    //TODO:  Process errors!

    if (params.avail_in != 0) {
        // Data was not completely consumed
        usebufa = !usebufa; // flip which buffer is in use
        std::memcpy(usebufa ? inbufa : inbufb, params.next_in, params.avail_in);
        avail_in = params.avail_in;
    } else
        avail_in = 0;

    // Flush bytes to ostream
    output->write(reinterpret_cast<const char *>(obuf), (long long)params.avail_out);
    params.avail_out = 0;
    params.next_out = obuf;
}

void WritesDeflate::write(std::uint8_t byte) {
    std::size_t i = avail_in;
    auto* buf = usebufa ? inbufa : inbufb;

    buf[i] = byte;
    avail_in++;

    if (avail_in == buf_size)
        flush();
}

void WritesDeflate::write(const std::uint8_t *array, std::size_t size) {
    std::size_t head = 0, i = avail_in;
    auto* buf = usebufa ? inbufa : inbufb;
    while (head <= size) {
        buf[i] = array[head];

        head++;
        i++;
        avail_in = i;

        if (i == buf_size)
            flush();
    }
}
