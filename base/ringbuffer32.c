// #include "stdint.h"
// #include "common/ringbuffer.h"
// #include "stdlib.h"
// #include "string.h"
// //#include "portable.h"

// #define CONVERT_TO_UINT32_POINTER(a) ((uint32_t *)((void *)(a)))
// #define CONVERT_TO_VOID_POINTER(a) ((void *)(a))
// #define POINTER_ADD(a, b) (((uint32_t *)((void *)(a))) + ((int32_t)(b)))

// // static inline int8_t RingBuffer8_IsSameReadWriteMirrorMode(RingBuffer *queue)
// // {
// //     return queue->readMirror == queue->writeMirror;
// // }

// void ringbuffer32_create(RingBuffer32 *queue, uint32_t *data, uint32_t maxSize)
// {
//     queue->size = maxSize;
//     queue->statusBits.overflowed = 0;
//     queue->data = data;
//     queue->write = 0;
//     queue->read = 0;
//     queue->OperationNotify = 0;
//     //queue->readMirror = 0;
//     //queue->writeMirror = 0;
// }

// void ringbuffer32_operation_notify_register(RingBuffer32 *queue, void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type))
// {
//     queue->OperationNotify = OperationNotify;
// }

// inline uint8_t ringbuffer32_is_full(RingBuffer32 *queue)
// {
//     return queue->write + 1 == queue->read;
// }

// inline uint8_t ringbuffer32_is_empty(RingBuffer32 *queue)
// {
//     return queue->write == queue->read;
//     //return (queue->write == queue->read) && (queue->readMirror == queue->writeMirror);
// }

// // inline uint8_t ringbuffer_space_enough(RingBuffer *queue, uint32_t length)
// // {
// //     int32_t len = queue->write - queue->read;
// //     if (len < 0)
// //     {
// //         len += queue->size;
// //     }
// //     return len;
// //     return (ringbuffer_count_get(queue) + length) <= queue->size;
// // }

// uint32_t ringbuffer32_mem_size_get(RingBuffer32 *queue)
// {
//     return queue->size;
// }

// inline uint32_t ringbuffer32_count_get(RingBuffer32 *queue)
// {
//     int32_t len = queue->write - queue->read;
//     if (len < 0)
//     {
//         len += queue->size;
//     }
//     return len;
// }

// inline uint8_t *ringbuffer32_write_ptr_get(RingBuffer32 *queue)
// {
//     return queue->data + ((queue->write >= queue->size) ? (queue->write - queue->size) : queue->write);
// }

// inline uint8_t *ringbuffer32_read_ptr_get(RingBuffer32 *queue)
// {
//     return queue->data + ((queue->read >= queue->size) ? (queue->read - queue->size) : queue->read);
// }

// uint8_t ringbuffer32_write_index_sync(RingBuffer32 *queue, uint32_t newWrite)
// {
//     uint32_t size = queue->size;
//     if (newWrite >= size)
//     {
//         newWrite %= size;
//     }
//     uint32_t write = queue->write;

//     int32_t writeDiff = newWrite - write;
//     uint32_t length = writeDiff >= 0 ? writeDiff : writeDiff + size;

//     uint32_t read = queue->read;
//     int32_t space = read - write - 1;
//     if (space <= -1)
//     {
//         space += size;
//     }

//     if (space < length)
//     {
//         queue->statusBits.overflowed = 1;
//         queue->read = newWrite + 1;
//     }
//     queue->write = newWrite;
//     if (queue->OperationNotify != NULL)
//     {
//         queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_HEAD);
//     }
//     return 0;
// }

// uint8_t ringbuffer32_read_index_sync(RingBuffer32 *queue, uint32_t newRead)
// {
//     uint32_t read = queue->read;

//     int32_t readDiff = newRead - read;
//     uint32_t length = readDiff >= 0 ? readDiff : readDiff + queue->size;

//     int32_t count = queue->write - read;
//     if (count < 0)
//     {
//         count += queue->size;
//     }
//     if (count < length)
//     {
//         return -1;
//     }
//     queue->read = newRead;

//     if (queue->OperationNotify != NULL)
//     {
//         queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_SYNC_TAIL);
//     }
//     return 0;
// }

// int32_t ringbuffer32_write(RingBuffer32 *queue, uint32_t *valuePtr, uint32_t length, uint8_t allowCoverTail)
// {
//     uint32_t write = queue->write;
//     uint32_t size = queue->size;
//     uint32_t cap = size - 1;
//     uint32_t rtnLength = length;
//     uint32_t read = queue->read;
//     int32_t space = read - write - 1;
//     if (space <= -1)
//     {
//         space += size;
//     }

//     if (space < length)
//     {
//         if (allowCoverTail)
//         {
//             if (length > cap)
//             {
//                 valuePtr = POINTER_ADD(valuePtr, (length - cap));
//                 length = cap;
//             }
//             queue->statusBits.overflowed = 1;
//         }
//         else
//         {
//             length = space;
//             rtnLength = length;
//         }
//     }

//     uint32_t spaceFromWriteToEnd = (read == 0) ? (cap - write) : (cap - write + 1);

//     if (length <= spaceFromWriteToEnd)
//     {
//         memcpy(POINTER_ADD(queue->data, write), valuePtr, length);
//         write += length;
//         write = (write == size) ? 0 : write;
//     }
//     else
//     {
//         memcpy(POINTER_ADD(queue->data, write), valuePtr, spaceFromWriteToEnd);
//         memcpy(queue->data, POINTER_ADD(valuePtr, spaceFromWriteToEnd), (length - spaceFromWriteToEnd));

//         write = length - spaceFromWriteToEnd;
//     }

//     if (space < length)
//     {
//         read = write + 1;
//         queue->read = read;
//     }
//     queue->write = write;

//     if (queue->OperationNotify != NULL)
//     {
//         queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_ENQUEUE);
//     }
//     return rtnLength;
// }

// int32_t ringbuffer32_read(RingBuffer32 *queue, uint32_t *valuePtr, uint32_t length)
// {
//     uint32_t *data = queue->data;
//     uint32_t size = queue->size;
//     uint32_t read = queue->read;
//     int32_t count = queue->write - read;
//     if (count < 0)
//     {
//         count += queue->size;
//     }
//     if (count < length)
//     {
//         length = count;
//     }
//     uint32_t countFormTailToBufferEnd = size - read;

//     if (countFormTailToBufferEnd >= length)
//     {
//         memcpy(valuePtr, data + read, length);
//         read += length;
//         read = (read >= size) ? (read - size) : read;
//     }
//     else
//     {
//         memcpy(valuePtr, data + read, countFormTailToBufferEnd);
//         memcpy(valuePtr + countFormTailToBufferEnd, data, (length - countFormTailToBufferEnd));

//         read = length - countFormTailToBufferEnd;
//     }

//     queue->read = read;
//     queue->statusBits.overflowed = 0;

//     if (queue->OperationNotify != NULL)
//     {
//         queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_DEQUEUE);
//     }
//     return length;
// }

// uint8_t ringbuffer32_peek_to_end(RingBuffer32 *queue, uint32_t **valuePtr, uint32_t *length)
// {
//     // uint32_t read = queue->read;
//     // uint32_t size = queue->size;
//     // uint32_t count = queue->count;
//     // uint32_t countFormTailToBufferEnd = size - read;

//     // *valuePtr = POINTER_ADD(queue->data, queue->read << unitSize);
//     // *length = count < countFormTailToBufferEnd ? count : countFormTailToBufferEnd;

//     // if (queue->OperationNotify != NULL)
//     // {
//     //     queue->OperationNotify(RINGBUFFER_OPERATION_TYPE_PEEK_TO_END);
//     // }
//     return 0;
// }
