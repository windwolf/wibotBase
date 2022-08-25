#include "ringbuffer.hpp"
#include "string.h"

#define CONVERT_TO_UINT8_POINTER(a) ((uint8_t *)((void *)(a)))
#define CONVERT_TO_VOID_POINTER(a) ((void *)(a))
#define POINTER_ADD(data, offset, width) ((uint8_t *)data + offset * width)

namespace ww
{

RingBuffer::RingBuffer(void *data, uint32_t dataWidth, uint32_t maxSize)
{
    _size = maxSize;
    _status.overflowed = 0;
    _data = data;
    _dataWidth = dataWidth;
    _write = 0;
    _read = 0;
    _operationNotify = 0;
};
bool RingBuffer::is_full()
{
    return _write + 1 == _read;
};
bool RingBuffer::is_empty()
{
    return _write == _read;
};
bool RingBuffer::is_space_enough(uint32_t length)
{
    return (_size - 1) - count_get() >= length;
};
uint32_t RingBuffer::mem_size_get()
{
    return _size * _dataWidth;
};
uint32_t RingBuffer::count_get()
{
    int32_t len = _write - _read;
    if (len < 0)
    {
        len += _size;
    }
    return len;
};
void *RingBuffer::write_ptr_get()
{
    return ((uint8_t *)_data) + ((_write >= _size) ? (_write - _size) : _write);
};
void *RingBuffer::read_ptr_get()
{
    return ((uint8_t *)_data) + ((_read >= _size) ? (_read - _size) : _read);
};
Result RingBuffer::write_index_sync(uint32_t newWrite)
{
    uint32_t size = _size;
    if (newWrite >= size)
    {
        newWrite %= size;
    }
    uint32_t write = _write;

    int32_t writeDiff = newWrite - write;
    uint32_t length = writeDiff >= 0 ? writeDiff : writeDiff + size;

    uint32_t read = _read;
    int32_t space = read - write - 1;
    if (space <= -1)
    {
        space += size;
    }

    if (space < (int32_t)length)
    {
        _status.overflowed = 1;
        _read = newWrite + 1;
    }
    _write = newWrite;
    if (_operationNotify != nullptr)
    {
        _operationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_HEAD);
    }
    return Result_OK;
};
Result RingBuffer::read_index_sync(uint32_t newRead)
{
    uint32_t read = _read;

    int32_t readDiff = newRead - read;
    uint32_t length = readDiff >= 0 ? readDiff : readDiff + _size;

    int32_t count = _write - read;
    if (count < 0)
    {
        count += _size;
    }
    if (count < (int32_t)length)
    {
        return Result_GeneralError;
    }
    _read = newRead;

    if (_operationNotify != nullptr)
    {
        _operationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_TAIL);
    }
    return Result_OK;
};
Result RingBuffer::read_offset_sync(uint32_t offset)
{
    if (offset > count_get())
    {
        return Result_InvalidParameter;
    }
    uint32_t index = _read + offset;
    if (index >= _size)
    {
        index -= _size;
    }
    _read = index;
    if (_operationNotify != nullptr)
    {
        _operationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_TAIL);
    }
    return Result_OK;
};
Result RingBuffer::write(void *valuePtr, uint32_t length,
                         uint8_t allowCoverTail, uint32_t *actualLength)
{
    if (length <= 0)
    {
        return Result_InvalidParameter;
    }
    uint32_t write = _write;
    uint32_t size = _size;
    uint32_t cap = size - 1;
    uint32_t read = _read;
    int32_t space = read - write - 1;
    if (space <= -1)
    {
        space += size;
    }

    if (space < (int32_t)length)
    {
        if (allowCoverTail)
        {
            if (length > cap)
            {
                valuePtr = POINTER_ADD(valuePtr, (length - cap), _dataWidth);
                length = cap;
            }
            _status.overflowed = 1;
        }
        else
        {
            length = space;
        }
    }

    uint32_t spaceFromWriteToEnd =
        (read == 0) ? (cap - write) : (cap - write + 1);

    if (length <= spaceFromWriteToEnd)
    {
        memcpy(POINTER_ADD(_data, write, _dataWidth), valuePtr, length);
        write += length;
        write = (write == size) ? 0 : write;
    }
    else
    {
        memcpy(POINTER_ADD(_data, write, _dataWidth), valuePtr,
               spaceFromWriteToEnd);
        memcpy(_data, POINTER_ADD(valuePtr, spaceFromWriteToEnd, _dataWidth),
               (length - spaceFromWriteToEnd));

        write = length - spaceFromWriteToEnd;
    }

    if (space < (int32_t)length)
    {
        read = write + 1;
        _read = read;
    }
    _write = write;

    if (_operationNotify != NULL)
    {
        _operationNotify(RINGBUFFER_OPERATION_TYPE_ENQUEUE);
    }
    *actualLength = length;
    return Result_OK;
};
Result RingBuffer::write_fill(uint8_t *value, uint32_t length,
                              uint8_t allowCoverTail, uint32_t *actualLength)
{
    if (length <= 0)
    {
        return Result_InvalidParameter;
    }
    uint32_t write = _write;
    uint32_t size = _size;
    uint32_t cap = size - 1;
    uint32_t read = _read;
    int32_t space = read - write - 1;
    if (space <= -1)
    {
        space += size;
    }

    if (space < (int32_t)length)
    {
        if (allowCoverTail)
        {
            if (length > cap)
            {
                length = cap;
            }
            _status.overflowed = 1;
        }
        else
        {
            length = space;
        }
    }

    uint32_t spaceFromWriteToEnd =
        (read == 0) ? (cap - write) : (cap - write + 1);

    if (length <= spaceFromWriteToEnd)
    {
        memset(POINTER_ADD(_data, write, _dataWidth), *value,
               length * _dataWidth);
        write += length;
        write = (write == size) ? 0 : write;
    }
    else
    {
        memset(POINTER_ADD(_data, write, _dataWidth), *value,
               spaceFromWriteToEnd * _dataWidth);
        memcpy(_data, (void *)value,
               (length - spaceFromWriteToEnd) * _dataWidth);

        write = length - spaceFromWriteToEnd;
    }

    if (space < (int32_t)length)
    {
        read = write + 1;
        _read = read;
    }
    _write = write;

    if (_operationNotify != NULL)
    {
        _operationNotify(RINGBUFFER_OPERATION_TYPE_ENQUEUE);
    }
    *actualLength = length;
    return Result_OK;
};
Result RingBuffer::read(void *valuePtr, uint32_t length, uint32_t &actualLength)
{
    uint8_t *data = (uint8_t *)_data;
    uint32_t size = _size;
    uint32_t read = _read;
    int32_t count = _write - read;
    if (count < 0)
    {
        count += _size;
    }
    if (count < (int32_t)length)
    {
        length = count;
    }
    uint32_t countFormTailToBufferEnd = size - read;

    if (countFormTailToBufferEnd >= length)
    {
        memcpy(valuePtr, data + read, length);
        read += length;
        read = (read >= size) ? (read - size) : read;
    }
    else
    {
        memcpy(valuePtr, POINTER_ADD(data, read, _dataWidth),
               countFormTailToBufferEnd * _dataWidth);
        memcpy(POINTER_ADD(valuePtr, countFormTailToBufferEnd, _dataWidth),
               data, (length - countFormTailToBufferEnd) * _dataWidth);

        read = length - countFormTailToBufferEnd;
    }

    _read = read;
    _status.overflowed = 0;

    if (_operationNotify != NULL)
    {
        _operationNotify(RINGBUFFER_OPERATION_TYPE_DEQUEUE);
    }
    actualLength = length;
    return Result_OK;
};
Result RingBuffer::peek(void *&data)
{
    if (is_empty())
    {
        return Result_GeneralError;
    }
    data = POINTER_ADD(_data, _read, _dataWidth);
    return Result_OK;
};
Result RingBuffer::index_peek(uint32_t index, void *&data)
{
    if (_write > _read)
    {
        if (index >= _write || index < _read)
        {
            return Result_InvalidParameter;
        }
    }
    else if (_write < _read)
    {
        if (index >= _write && index < _read)
        {
            return Result_InvalidParameter;
        }
    }
    else
    {
        return Result_InvalidParameter;
    }
    data = POINTER_ADD(_data, index, _dataWidth);
    return Result_OK;
};
Result RingBuffer::offset_peek(uint32_t offset, void *&data)
{
    if (offset > count_get())
    {
        return Result_InvalidParameter;
    }
    uint32_t index = _read + offset;
    if (index >= _size)
    {
        index -= _size;
    }
    data = POINTER_ADD(_data, index, _dataWidth);
    return Result_OK;
};
void RingBuffer::operation_notify_register(OperationNotify operationNotify)
{
    _operationNotify = operationNotify;
};

void *RingBuffer::index_peek_directly(uint32_t index)
{
    return POINTER_ADD(_data, index, _dataWidth);
};
void *RingBuffer::offset_peek_directly(uint32_t offset)
{
    uint32_t index = _read + offset;
    if (index >= _size)
    {
        index -= _size;
    }
    return POINTER_ADD(_data, index, _dataWidth);
};
uint32_t RingBuffer::offset_to_index_convert(uint32_t offset)
{
    uint32_t index = _read + offset;
    if (index >= _size)
    {
        index -= _size;
    }
    return index;
};
uint32_t RingBuffer::index_wrap(int32_t index)
{
    if (index >= (int32_t)_size)
    {
        index %= _size;
    }
    if (index < 0)
    {
        index += _size;
    }
    return index;
};

void *RingBuffer::data_ptr_get()
{
    return _data;
};

} // namespace ww
