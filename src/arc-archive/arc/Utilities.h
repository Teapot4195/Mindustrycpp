//
// Created by teapot on 20/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_UTILITIES_H
#define MINDUSTRY_SRC_ARC_UTILITIES_H

#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

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

    std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }

        return str;
    }
}

/**
 * Buffer utils, utility functions for java.nio etc.
 */
namespace bu {
    template <typename Backer, typename This>
    struct bufview;

    /**
     * @brief buffer like structure similar to java.nio.ByteBuffer
     *
     * Most functions returning self actually returns this&.
     *
     * @tparam T underlying type of this buffer
     */
    template <typename T>
    struct buffer : public std::enable_shared_from_this<buffer<T>> {
        std::vector<T> backer;
        std::size_t head{}; /// Position of this buffer
        std::size_t mark_{}; /// Mark Position
        std::size_t limit_{}; /// Limit of this buffer
        std::size_t cap{}; /// Maximum capacity of this buffer
        bool marked{false}; /// Whether or not this buffer is marked

        explicit buffer(std::size_t cap) : cap(cap) {
            backer = std::vector<T>(cap);
        }
        ~buffer() = default;

        template <typename TType>
        bufview<T, TType> asview();

        /**
         * @return returns the underlying vector that backs up this buffer.
         */
        std::vector<T>& array() {
            return backer;
        }

        /**
         * @brief Returns this buffer's capacity, cannot be changed.
         *
         * @return returns the capacity of this buffer.
         */
        [[nodiscard]] int capacity() const {
            return capacity;
        }

        /**
         * Returns the current position of this buffer head
         *
         * @return Buffer head position
         */
         [[nodiscard]] int position() const {
             return head;
         }

         /**
          * Sets this buffer's position. If the mark is defined and larger than the new position then it is discarded.
          *
          * @returns this buffer
          */
         buffer<T>& position(int newPosition) {
             if (newPosition < 0)
                 throw std::invalid_argument("newPosition must be nonnegative.");
             if (newPosition > limit_)
                 throw std::invalid_argument("newPosition must be less than limit.");
             head = newPosition;
             if (marked && mark_ > newPosition) // TODO: Can we ignore the marked check?
                 marked = false;
             return *this;
         }

         /**
          * Returns the current limit for this buffer
          *
          * @return Buffer limit
          */
         [[nodiscard]] int limit() const {
             return limit_;
         }

         /**
          * Sets this buffer's limit. If the position is larger than the new limit then it is set to the new limit.
          * If the mark is defined and larger than the new limit then it is discarded.
          *
          * @returns this buffer
          */
          buffer<T>& limit(int newLimit) {
              if (newLimit < 0)
                  throw std::invalid_argument("newLimit must be nonnegative.");
              if (newLimit > cap)
                  throw std::invalid_argument("newLimit must be less than capacity");
              limit_ = newLimit;
              if (marked && mark_ > newLimit) // TODO: Same as setPosition?
                  marked = false;
             return *this;
          }

          /**
           * Sets this buffer's mark at its position.
           *
           * @returns this buffer
           */
           buffer<T>& mark() {
               marked = true;
               mark_ = head;
               return *this;
           }

           /**
            * Resets this buffer's position to the previously-marked position.
            * Invoking this method neither changes nor discards the mark's value.
            */
            buffer<T>& reset() {
                if (!marked)
                    throw std::invalid_argument("Set the mark first before calling reset!");
                head = mark_;
                return *this;
            }

            /**
             * Clears this buffer. The position is set to zero, the limit is set to the capacity, and the mark is discarded.
             *
             * Invoke this method before using a sequence of channel-read or put operations to fill this buffer. For example:
             *
             *      buf.clear();     // Prepare buffer for reading
             *      in.read(buf);    // Read data
             *
             * This method does not actually erase the data in the buffer, but it is named as if it did because it will
             * most often be used in situations in which that might as well be the case.
             */
             buffer<T>& clear() noexcept {
                 head = 0;
                 limit_ = cap;
                 marked = false;
                 return *this;
             }

             /**
              * Flips this buffer. The limit is set to the current position and then the position is set to zero. If the mark is defined then it is discarded.
              *
              * After a sequence of channel-read or put operations, invoke this method to prepare for a sequence of channel-write or relative get operations. For example:
              *
              *      buf.put(magic);    // Prepend header
              *      in.read(buf);      // Read data into rest of buffer
              *      buf.flip();        // Flip buffer
              *      out.write(buf);    // Write header + data to channel
              *
              * This method is often used in conjunction with the compact method when transferring data from one place to another.
              */
             buffer<T>& flip() noexcept {
                 limit_ = head;
                 head = 0;
                 marked = false;
                 return *this;
             }

             // TODO: Too lazy to document
             buffer<T>& rewind() noexcept {
                 head = 0;
                 marked = false;
             }

             [[nodiscard]] int remaining() const {
                 return limit_ - head;
             }

             [[nodiscard]] bool hasRemaining() const {
                 return (limit_ - head) != 0;
             }
    };

    template <typename Backer, typename This>
    struct bufview {
        std::shared_ptr<buffer<Backer>> b;
        std::vector<Backer>& backer;

        std::size_t head{}; /// Position of this buffer
        std::size_t mark_{}; /// Mark Position
        std::size_t limit_{}; /// Limit of this buffer
        bool marked{false}; /// Whether or not this buffer is marked

        explicit bufview(std::shared_ptr<buffer<Backer>> bac) : b(bac), backer(bac) {}

        ~bufview() = default;

        template <typename TType>
        bufview<Backer, TType> asview();

        /**
         * @return returns the underlying vector that backs up this buffer.
         */
        std::vector<Backer>& array() {
            return backer;
        }

        /**
         * @brief Returns this buffer's capacity, cannot be changed.
         *
         * @return returns the capacity of this buffer.
         */
        [[nodiscard]] int capacity() const {
            return b->cap;
        }

        /**
         * Returns the current position of this buffer head
         *
         * @return Buffer head position
         */
        [[nodiscard]] int position() const {
            return head;
        }

        /**
         * Sets this buffer's position. If the mark is defined and larger than the new position then it is discarded.
         *
         * @returns this buffer
         */
        buffer<Backer>& position(int newPosition) {
            if (newPosition < 0)
                throw std::invalid_argument("newPosition must be nonnegative.");
            if (newPosition > limit_)
                throw std::invalid_argument("newPosition must be less than limit.");
            head = newPosition;
            if (marked && mark_ > newPosition) // TODO: Can we ignore the marked check?
                marked = false;
            return *this;
        }

        /**
         * Returns the current limit for this buffer
         *
         * @return Buffer limit
         */
        [[nodiscard]] int limit() const {
            return limit_;
        }

        /**
         * Sets this buffer's limit. If the position is larger than the new limit then it is set to the new limit.
         * If the mark is defined and larger than the new limit then it is discarded.
         *
         * @returns this buffer
         */
        buffer<Backer>& limit(int newLimit) {
            if (newLimit < 0)
                throw std::invalid_argument("newLimit must be nonnegative.");
            if (newLimit > b->cap)
                throw std::invalid_argument("newLimit must be less than capacity");
            limit_ = newLimit;
            if (marked && mark_ > newLimit) // TODO: Same as setPosition?
                marked = false;
            return *this;
        }

        /**
         * Sets this buffer's mark at its position.
         *
         * @returns this buffer
         */
        buffer<Backer>& mark() {
            marked = true;
            mark_ = head;
            return *this;
        }

        /**
         * Resets this buffer's position to the previously-marked position.
         * Invoking this method neither changes nor discards the mark's value.
         */
        buffer<Backer>& reset() {
            if (!marked)
                throw std::invalid_argument("Set the mark first before calling reset!");
            head = mark_;
            return *this;
        }

        /**
         * Clears this buffer. The position is set to zero, the limit is set to the capacity, and the mark is discarded.
         *
         * Invoke this method before using a sequence of channel-read or put operations to fill this buffer. For example:
         *
         *      buf.clear();     // Prepare buffer for reading
         *      in.read(buf);    // Read data
         *
         * This method does not actually erase the data in the buffer, but it is named as if it did because it will
         * most often be used in situations in which that might as well be the case.
         */
        buffer<Backer>& clear() noexcept {
            head = 0;
            limit_ = b->cap;
            marked = false;
            return *this;
        }

        /**
         * Flips this buffer. The limit is set to the current position and then the position is set to zero. If the mark is defined then it is discarded.
         *
         * After a sequence of channel-read or put operations, invoke this method to prepare for a sequence of channel-write or relative get operations. For example:
         *
         *      buf.put(magic);    // Prepend header
         *      in.read(buf);      // Read data into rest of buffer
         *      buf.flip();        // Flip buffer
         *      out.write(buf);    // Write header + data to channel
         *
         * This method is often used in conjunction with the compact method when transferring data from one place to another.
         */
        buffer<Backer>& flip() noexcept {
            limit_ = head;
            head = 0;
            marked = false;
            return *this;
        }

        // TODO: Too lazy to document
        buffer<Backer>& rewind() noexcept {
            head = 0;
            marked = false;
        }

        [[nodiscard]] int remaining() const {
            return limit_ - head;
        }

        [[nodiscard]] bool hasRemaining() const {
            return (limit_ - head) != 0;
        }

        [[nodiscard]] unsigned char get() {
            if (head >= b->cap)
                throw std::invalid_argument("End of buffer");
            return b->backer[head++];
        }

        buffer<Backer>& get(std::vector<unsigned char>& dst) {

        }
    };

    template<typename Backer, typename This>
    template<typename TType>
    bufview<Backer, TType> bufview<Backer, This>::asview() {
        return b->template asview<TType>();
    }

    template<typename T>
    template<typename TType>
    bufview<T, TType> buffer<T>::asview() {
        return bufview<T, TType>(this->shared_from_this());
    }
}

#endif //MINDUSTRY_SRC_ARC_UTILITIES_H
