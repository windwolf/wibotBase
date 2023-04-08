//
// Created by zhouj on 2023/3/29.
//

#ifndef WWMOTOR_LIBS_WWBASE_BASE_CIRCULARBUFFER_HPP_
#define WWMOTOR_LIBS_WWBASE_BASE_CIRCULARBUFFER_HPP_

#include <algorithm>

#include "base.hpp"
#include "stdint.h"

#define WRAP_LOGIC_INDEX(a) ((a) & (2 * _capacity - 1))
#define WRAP_MEM_INDEX(a)   ((a) & (_capacity - 1))

namespace wibot {
/**
 * Circular buffer implementation
 * @tparam TE
 */
template <typename TE>
class CircularBuffer {
   private:
    TE      *_buffer;
    uint32_t _capacity;
    uint32_t _write;
    uint32_t _read;

   public:
    /**
     * @brief Construct a new Circular Buffer object.
     *
     * @param buffer underline buffer.
     * @param capacity Underline buffer size. Must be power of 2.
     */
    CircularBuffer(TE *buffer, uint32_t capacity);
    ;
    uint32_t getCapacity() {
        return _capacity;
    };
    uint32_t getMemCapacity() {
        return _capacity * sizeof(TE);
    };
    uint32_t getDataWidth() {
        return sizeof(TE);
    };

    bool isFull() {
        return _read == (_write ^ _capacity);
    };

    bool isEmpty() {
        return _write == _read;
    };

    uint32_t getSize() {
        return (_write - _read) & ((_capacity << 1) - 1);
    };

    uint32_t getSpace() {
        return _capacity - getSize();
    };

    /**
     * Wirte data into buffer
     * @param data Data to be written.
     * @param length Length of data to be written.
     * @param allowCover Allow cover the data in buffer if the buffer is full.
     * If not, only the data that can be written will be written.
     * @note If allowCover is true, this function will modify the read index,
     * and user code should handle the concurrent problem.
     * @return The length of data that is written into buffer.
     */
    uint32_t write(TE *data, uint32_t length, bool allowCover = true);
    ;

    /**
     * Forward the write index after the data is written into the buffer by the
     * external device such as DMA.
     * @note  User code should handle the concurrency issue, if the start index
     * is pushed over the end index.
     * @remark This function is mainly used for DMA transfer, so it can not
     * avoid the data cover issue. But generally speaking, the DMA speed is much
     * slower than the CPU, so it is not easy to cause data cover.
     * @param length The length of data that has been written into buffer by the
     * external device.
     * @return If overflow occurs, return true, otherwise return false.
     */
    bool writeVirtual(uint32_t length);
    ;

    /**
     * Read data from buffer. If buffer has enough
     * @param data
     * @param length
     * @return Actual length of data readed.
     */
    uint32_t read(TE *data, uint32_t length);

    /**
     * @brief Forward the read index after the data is readed from the buffer by
     * the external device such as DMA.
     * @note User code should handle the concurrency issue, if the start index
     * is pushed over the end index.
     * @param length The length of data that has been read from buffer.
     * @return If overflow occurs, return true, otherwise return false.
     * */
    bool readVirtual(uint32_t length);

    /**
     * @brief Read data from buffer without changing the read index.
     *
     * @param data A buffer to store the data readed.
     * @param start The start index of data to be readed.
     * @param length The length of data to be readed.
     * @return The actual length of data has been readed.
     */
    uint32_t peek(TE *data, uint32_t start = 0, uint32_t length = 1);
    ;

    /**
     * @brief Get the pointer of the data at the specified offset.
     *
     * @param offset
     * @return TE*
     */
    TE *peekPtr(uint32_t offset, bool force = false) {
        if (force) {
            return &_buffer[WRAP_MEM_INDEX(_read)];
        } else {
            if (offset >= getSize()) {
                return nullptr;
            }
            return &_buffer[WRAP_MEM_INDEX(_read + offset)];
        }
    }

    Result clear() {
        _write = 0;
        _read  = 0;
        return Result::OK;
    };

    TE *getDataPtr() {
        return _buffer;
    };

    TE *getWritePtr() {
        return &_buffer[WRAP_MEM_INDEX(_write)];
    };

    TE *getReadPtr() {
        return &_buffer[WRAP_MEM_INDEX(_read)];
    };

    /**
     * @brief Get the size of data from read index to the end of the buffer or data.
     * Used for zero copy operation.
     * @return If data size is smaller than the rest room from read index to the end, return the
     * data size, otherwise return the rest room size.
     */
    uint32_t getSizeWithoutMemWrap() {
        return min((_write - _read) & ((_capacity << 1) - 1), _capacity - WRAP_MEM_INDEX(_read));
    };

    uint32_t getLengthByMemIndex(uint32_t end, uint32_t start) {
        return (end - start) & (_capacity - 1);
    }
};

using CircularBuffer8 = CircularBuffer<uint8_t>;

}  // namespace wibot

#endif  // WWMOTOR_LIBS_WWBASE_BASE_CIRCULARBUFFER_HPP_
