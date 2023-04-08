//
// Created by zhouj on 2023/4/8.
//

#include "CircularBuffer.hpp"
#include "log.h"
LOGGER("cb")

template <typename TE>
uint32_t wibot::CircularBuffer<TE>::peek(TE *data, uint32_t start, uint32_t length) {
    auto size = getSize();
    if (start >= size) {
        return 0;
    }
    if (length > size - start) {
        length = size - start;
    }
    auto read_mem_index       = WRAP_MEM_INDEX(_read + start);
    auto roomFromReadToBotton = _capacity - read_mem_index;
    if (length <= roomFromReadToBotton) {
        memcpy(data, _buffer + read_mem_index, length * sizeof(TE));
    } else {
        memcpy(data, _buffer + read_mem_index, roomFromReadToBotton * sizeof(TE));
        memcpy(data + roomFromReadToBotton, _buffer, (length - roomFromReadToBotton) * sizeof(TE));
    }
    return length;
}
template <typename TE>
uint32_t wibot::CircularBuffer<TE>::read(TE *data, uint32_t length) {
    auto size = getSize();
    if (length > size) {
        length = size;
    }
    auto read_mem_index       = WRAP_MEM_INDEX(_read);
    auto roomFromReadToBotton = _capacity - read_mem_index;
    if (length <= roomFromReadToBotton) {
        memcpy(data, _buffer + read_mem_index, length * sizeof(TE));
    } else {
        memcpy(data, _buffer + read_mem_index, roomFromReadToBotton * sizeof(TE));
        memcpy(data + roomFromReadToBotton, _buffer, (length - roomFromReadToBotton) * sizeof(TE));
    }
    _read = WRAP_LOGIC_INDEX(_read + length);
    return length;
}
template <typename TE>
bool wibot::CircularBuffer<TE>::readVirtual(uint32_t length) {
    auto size     = getSize();
    auto overflow = false;
    if (length > size) {
        overflow = true;
        length   = size;
    }
    _read = WRAP_LOGIC_INDEX(_read + length);
    if (overflow) {
        _write = _read;
    }
    return overflow;
}
template <typename TE>
bool wibot::CircularBuffer<TE>::writeVirtual(uint32_t length) {
    auto space    = getSpace();
    auto overflow = false;
    if (length > space) {
        if (length > _capacity) {
            length = _capacity;
        }
        overflow = true;
    }
    _write = WRAP_LOGIC_INDEX(_write + length);
    if (overflow) {
        _read = WRAP_LOGIC_INDEX(_write - _capacity);
    }
    return overflow;
}
template <typename TE>
uint32_t wibot::CircularBuffer<TE>::write(TE *data, uint32_t length, bool allowCover) {
    auto overflow = false;
    auto space    = getSpace();
    if (length > space) {
        if (allowCover) {
            if (length > _capacity) {
                length = _capacity;
                data += length - _capacity;
            }
            overflow = true;
        } else {
            length = space;
        }
    } else {
    }
    auto write_mem_index       = WRAP_MEM_INDEX(_write);
    auto roomFromWriteToBotton = _capacity - write_mem_index;
    if (length <= roomFromWriteToBotton) {
        memcpy(_buffer + write_mem_index, data, length * sizeof(TE));
    } else {
        memcpy(_buffer + write_mem_index, data, roomFromWriteToBotton * sizeof(TE));
        memcpy(_buffer, data + roomFromWriteToBotton,
               (length - roomFromWriteToBotton) * sizeof(TE));
    }
    _write = WRAP_LOGIC_INDEX(_write + length);

    if (overflow) {
        _read = WRAP_LOGIC_INDEX(_write - _capacity);
    }

    return length;
}
template <typename TE>
wibot::CircularBuffer<TE>::CircularBuffer(TE *buffer, uint32_t capacity)
    : _buffer(buffer), _capacity(capacity) {
    ASSERT(((capacity - 1) ^ capacity) == ((capacity - 1) | capacity),
           "Buffer size MUST be power of 2!");
    _write = 0;
    _read  = 0;
}

template class wibot::CircularBuffer<uint8_t>;
