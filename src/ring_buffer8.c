#include "stdint.h"
#include "basic/ring_buffer8.h"
#include "stdlib.h"
#include "string.h"
//#include "portable.h"

#define CONVERT_TO_UINT8_POINTER(a) ((uint8_t *)((void *)(a)))
#define CONVERT_TO_VOID_POINTER(a) ((void *)(a))
#define POINTER_ADD(a, b) (((uint8_t *)((void *)(a))) + ((int32_t)(b)))

static inline uint32_t RinBuffer8_Count(RingBuffer8 *queue)
{
    int32_t len = queue->write - queue->read;
    if (len < 0)
    {
        len += queue->size;
    }
    return len;
}

// static inline int8_t RingBuffer8_IsSameReadWriteMirrorMode(RingBuffer8 *queue)
// {
//     return queue->readMirror == queue->writeMirror;
// }

void RingBuffer8_Create(RingBuffer8 *queue, uint8_t *data, uint32_t maxSize)
{
    queue->size = maxSize;
    queue->statusBits.overflowed = 0;
    queue->data = data;
    queue->write = 0;
    queue->read = 0;
    queue->OperationNotify = 0;
    //queue->readMirror = 0;
    //queue->writeMirror = 0;
}

void RingBuffer8_RegisterOperationNotify(RingBuffer8 *queue, void (*OperationNotify)(RingBuffer8_OperationType type))
{
    queue->OperationNotify = OperationNotify;
}

inline uint8_t RingBuffer8_IsFull(RingBuffer8 *queue)
{
    return queue->write + 1 == queue->read;
}

inline uint8_t RingBuffer8_IsEmpty(RingBuffer8 *queue)
{
    return queue->write == queue->read;
    //return (queue->write == queue->read) && (queue->readMirror == queue->writeMirror);
}

// inline uint8_t RingBuffer8_IsEnoughSpace(RingBuffer8 *queue, uint32_t length)
// {
//     int32_t len = queue->write - queue->read;
//     if (len < 0)
//     {
//         len += queue->size;
//     }
//     return len;
//     return (RingBuffer8_GetCount(queue) + length) <= queue->size;
// }

uint32_t RingBuffer8_GetMemorySize(RingBuffer8 *queue)
{
    return queue->size;
}

inline uint32_t RingBuffer8_GetCount(RingBuffer8 *queue)
{
    int32_t len = queue->write - queue->read;
    if (len < 0)
    {
        len += queue->size;
    }
    return len;
}

inline uint8_t *RingBuffer8_GetWrite(RingBuffer8 *queue)
{
    return queue->data + ((queue->write >= queue->size) ? (queue->write - queue->size) : queue->write);
}

inline uint8_t *RingBuffer8_GetRead(RingBuffer8 *queue)
{
    return queue->data + ((queue->read >= queue->size) ? (queue->read - queue->size) : queue->read);
}

uint8_t RingBuffer8_SyncWrite(RingBuffer8 *queue, uint32_t newWrite)
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
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_SYNC_HEAD);
    }
    return 0;
}

uint8_t RingBuffer8_SyncRead(RingBuffer8 *queue, uint32_t newRead)
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
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_SYNC_TAIL);
    }
    return 0;
}

int32_t RingBuffer8_Write(RingBuffer8 *queue, uint8_t *valuePtr, uint32_t length, uint8_t allowCoverTail)
{
    uint32_t write = queue->write;
    uint32_t size = queue->size;
    uint32_t cap = size - 1;
    uint32_t rtnLength = length;
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
                valuePtr = POINTER_ADD(valuePtr, (length - cap));
                length = cap;
            }
            queue->statusBits.overflowed = 1;
        }
        else
        {
            length = space;
            rtnLength = length;
        }
    }

    uint32_t spaceFromWriteToEnd = (read == 0) ? (cap - write) : (cap - write + 1);

    if (length <= spaceFromWriteToEnd)
    {
        memcpy(POINTER_ADD(queue->data, write), valuePtr, length);
        write += length;
        write = (write == size) ? 0 : write;
    }
    else
    {
        memcpy(POINTER_ADD(queue->data, write), valuePtr, spaceFromWriteToEnd);
        memcpy(queue->data, POINTER_ADD(valuePtr, spaceFromWriteToEnd), (length - spaceFromWriteToEnd));

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
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_ENQUEUE);
    }
    return rtnLength;
}

int32_t RingBuffer8_Read(RingBuffer8 *queue, uint8_t *valuePtr, uint32_t length)
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
        queue->OperationNotify(CIRCLE_BUFFER_OPERATION_DEQUEUE);
    }
    return length;
}

uint8_t RingBuffer8_PeekToEnd(RingBuffer8 *queue, uint8_t **valuePtr, uint32_t *length)
{
    // uint32_t read = queue->read;
    // uint32_t size = queue->size;
    // uint32_t count = queue->count;
    // uint32_t countFormTailToBufferEnd = size - read;

    // *valuePtr = POINTER_ADD(queue->data, queue->read << unitSize);
    // *length = count < countFormTailToBufferEnd ? count : countFormTailToBufferEnd;

    // if (queue->OperationNotify != NULL)
    // {
    //     queue->OperationNotify(CIRCLE_BUFFER_OPERATION_PEEK_TO_END);
    // }
    return 0;
}
