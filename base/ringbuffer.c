#include "stdint.h"
#include "ringbuffer.h"
#include "stdlib.h"
#include "string.h"
//#include "portable.h"

#define CONVERT_TO_UINT8_POINTER(a) ((uint8_t *)((void *)(a)))
#define CONVERT_TO_VOID_POINTER(a) ((void *)(a))
#define POINTER_ADD(data, offset, width) ((uint8_t *)data + offset * width)

// static inline int8_t RingBuffer8_IsSameReadWriteMirrorMode(RingBuffer *queue)
// {
//     return queue->readMirror == queue->writeMirror;
// }

void ringbuffer_create(RingBuffer *queue, void *data, uint32_t dataWidth, uint32_t size)
{
    queue->size = size;
    queue->statusBits.overflowed = 0;
    queue->data = data;
    queue->dataWidth = dataWidth;
    queue->write = 0;
    queue->read = 0;
    queue->OperationNotify = 0;
}

void ringbuffer_operation_notify_register(RingBuffer *queue, void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type))
{
    queue->OperationNotify = OperationNotify;
}

inline bool ringbuffer_is_full(RingBuffer *queue)
{
    return queue->write + 1 == queue->read;
}

inline bool ringbuffer_is_empty(RingBuffer *queue)
{
    return queue->write == queue->read;
    // return (queue->write == queue->read) && (queue->readMirror == queue->writeMirror);
}

inline bool ringbuffer_space_enough(RingBuffer *queue, uint32_t length)
{
    return (queue->size - 1) - ringbuffer_count_get(queue) >= length;
}

uint32_t ringbuffer_mem_size_get(RingBuffer *queue)
{
    return queue->size * queue->dataWidth;
}

inline uint32_t ringbuffer_count_get(RingBuffer *queue)
{
    int32_t len = queue->write - queue->read;
    if (len < 0)
    {
        len += queue->size;
    }
    return len;
}

inline void *ringbuffer_write_ptr_get(RingBuffer *queue)
{
    return ((uint8_t *)queue->data) + ((queue->write >= queue->size) ? (queue->write - queue->size) : queue->write);
}

inline void *ringbuffer_read_ptr_get(RingBuffer *queue)
{
    return ((uint8_t *)queue->data) + ((queue->read >= queue->size) ? (queue->read - queue->size) : queue->read);
}

OP_RESULT ringbuffer_write_index_sync(RingBuffer *queue, uint32_t newWrite)
{
    uint32_t size = queue->size;
    if (newWrite >= size)
    {
        newWrite %= size;
    }
    uint32_t write = queue->write;

    int32_t writeDiff = newWrite - write;
    uint32_t length = writeDiff >= 0 ? writeDiff : writeDiff + size;

    uint32_t read = queue->read;
    int32_t space = read - write - 1;
    if (space <= -1)
    {
        space += size;
    }

    if (space < length)
    {
        queue->statusBits.overflowed = 1;
        queue->read = newWrite + 1;
    }
    queue->write = newWrite;
    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_HEAD);
    }
    return OP_RESULT_OK;
}

OP_RESULT ringbuffer_read_index_sync(RingBuffer *queue, uint32_t newRead)
{
    uint32_t read = queue->read;

    int32_t readDiff = newRead - read;
    uint32_t length = readDiff >= 0 ? readDiff : readDiff + queue->size;

    int32_t count = queue->write - read;
    if (count < 0)
    {
        count += queue->size;
    }
    if (count < length)
    {
        return -1;
    }
    queue->read = newRead;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_TAIL);
    }
    return OP_RESULT_OK;
}

OP_RESULT ringbuffer_read_offset_sync(RingBuffer *queue, uint32_t offset)
{
    if (offset > ringbuffer_count_get(queue))
    {
        return OP_RESULT_PARAMETER_ERROR;
    }
    uint32_t index = queue->read + offset;
    if (index >= queue->size)
    {
        index -= queue->size;
    }
    queue->read = index;
    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_TAIL);
    }
    return OP_RESULT_OK;
};

OP_RESULT ringbuffer_write(RingBuffer *queue, void *valuePtr, uint32_t length, uint8_t allowCoverTail, uint32_t *actualLength)
{
    if (length <= 0)
    {
        return OP_RESULT_PARAMETER_ERROR;
    }
    uint32_t write = queue->write;
    uint32_t size = queue->size;
    uint32_t cap = size - 1;
    uint32_t read = queue->read;
    int32_t space = read - write - 1;
    if (space <= -1)
    {
        space += size;
    }

    if (space < length)
    {
        if (allowCoverTail)
        {
            if (length > cap)
            {
                valuePtr = POINTER_ADD(valuePtr, (length - cap), queue->dataWidth);
                length = cap;
            }
            queue->statusBits.overflowed = 1;
        }
        else
        {
            length = space;
        }
    }

    uint32_t spaceFromWriteToEnd = (read == 0) ? (cap - write) : (cap - write + 1);

    if (length <= spaceFromWriteToEnd)
    {
        memcpy(POINTER_ADD(queue->data, write, queue->dataWidth), valuePtr, length);
        write += length;
        write = (write == size) ? 0 : write;
    }
    else
    {
        memcpy(POINTER_ADD(queue->data, write, queue->dataWidth), valuePtr, spaceFromWriteToEnd);
        memcpy(queue->data, POINTER_ADD(valuePtr, spaceFromWriteToEnd, queue->dataWidth), (length - spaceFromWriteToEnd));

        write = length - spaceFromWriteToEnd;
    }

    if (space < length)
    {
        read = write + 1;
        queue->read = read;
    }
    queue->write = write;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_ENQUEUE);
    }
    *actualLength = length;
    return OP_RESULT_OK;
}

OP_RESULT ringbuffer_write_fill(RingBuffer *queue, uint8_t *value, uint32_t length, uint8_t allowCoverTail, uint32_t *actualLength)
{
    if (length <= 0)
    {
        return OP_RESULT_PARAMETER_ERROR;
    }
    uint32_t write = queue->write;
    uint32_t size = queue->size;
    uint32_t cap = size - 1;
    uint32_t read = queue->read;
    int32_t space = read - write - 1;
    if (space <= -1)
    {
        space += size;
    }

    if (space < length)
    {
        if (allowCoverTail)
        {
            if (length > cap)
            {
                length = cap;
            }
            queue->statusBits.overflowed = 1;
        }
        else
        {
            length = space;
        }
    }

    uint32_t spaceFromWriteToEnd = (read == 0) ? (cap - write) : (cap - write + 1);

    if (length <= spaceFromWriteToEnd)
    {
        memset(POINTER_ADD(queue->data, write, queue->dataWidth), *value, length * queue->dataWidth);
        write += length;
        write = (write == size) ? 0 : write;
    }
    else
    {
        memset(POINTER_ADD(queue->data, write, queue->dataWidth), *value, spaceFromWriteToEnd * queue->dataWidth);
        memcpy(queue->data, (void *)value, (length - spaceFromWriteToEnd) * queue->dataWidth);

        write = length - spaceFromWriteToEnd;
    }

    if (space < length)
    {
        read = write + 1;
        queue->read = read;
    }
    queue->write = write;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_ENQUEUE);
    }
    *actualLength = length;
    return OP_RESULT_OK;
}

OP_RESULT ringbuffer_read(RingBuffer *queue, void *valuePtr, uint32_t length, uint32_t *actualLegnth)
{
    uint8_t *data = queue->data;
    uint32_t size = queue->size;
    uint32_t read = queue->read;
    int32_t count = queue->write - read;
    if (count < 0)
    {
        count += queue->size;
    }
    if (count < length)
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
        memcpy(valuePtr, data + read, countFormTailToBufferEnd);
        memcpy(valuePtr + countFormTailToBufferEnd, data, (length - countFormTailToBufferEnd));

        read = length - countFormTailToBufferEnd;
    }

    queue->read = read;
    queue->statusBits.overflowed = 0;

    if (queue->OperationNotify != NULL)
    {
        queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_DEQUEUE);
    }
    *actualLegnth = length;
    return OP_RESULT_OK;
}

OP_RESULT ringbuffer_index_peek(RingBuffer *queue, uint32_t index, void **data)
{
    if (queue->write > queue->read)
    {
        if (index >= queue->write || index < queue->read)
        {
            return OP_RESULT_PARAMETER_ERROR;
        }
    }
    else if (queue->write < queue->read)
    {
        if (index >= queue->write && index < queue->read)
        {
            return OP_RESULT_PARAMETER_ERROR;
        }
    }
    else
    {
        return OP_RESULT_PARAMETER_ERROR;
    }
    *data = POINTER_ADD(queue->data, index, queue->dataWidth);
    return OP_RESULT_OK;
};

OP_RESULT ringbuffer_offset_peek(RingBuffer *queue, uint32_t offset, void **data)
{
    if (offset > ringbuffer_count_get(queue))
    {
        return OP_RESULT_PARAMETER_ERROR;
    }
    uint32_t index = queue->read + offset;
    if (index >= queue->size)
    {
        index -= queue->size;
    }
    *data = POINTER_ADD(queue->data, index, queue->dataWidth);
    return OP_RESULT_OK;
};

OP_RESULT ringbuffer_peek(RingBuffer *queue, void **data)
{
    if (ringbuffer_is_empty(queue))
    {
        return OP_RESULT_GENERAL_ERROR;
    }
    *data = POINTER_ADD(queue->data, queue->read, queue->dataWidth);
    return 0;
}

ALWAYS_INLINE void *ringbuffer_index_peek_directly(RingBuffer *queue, uint32_t index)
{
    return POINTER_ADD(queue->data, index, queue->dataWidth);
};

ALWAYS_INLINE void *ringbuffer_offset_peek_directly(RingBuffer *queue, uint32_t offset)
{
    uint32_t index = queue->read + offset;
    if (index >= queue->size)
    {
        index -= queue->size;
    }
    return POINTER_ADD(queue->data, index, queue->dataWidth);
};

ALWAYS_INLINE uint32_t ringbuffer_offset_to_index_convert(RingBuffer *queue, uint32_t offset)
{
    uint32_t index = queue->read + offset;
    if (index >= queue->size)
    {
        index -= queue->size;
    }
    return index;
}

ALWAYS_INLINE uint32_t ringbuffer_index_wrap(RingBuffer *queue, int32_t index)
{
    if (index >= queue->size)
    {
        index %= queue->size;
    }
    if (index < 0)
    {
        index += queue->size;
    }
    return index;
}
