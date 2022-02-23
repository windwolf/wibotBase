#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "shared.h"
    typedef enum RINGBUFFER_OPERATION_TYPE
    {
        RINGBUFFER_OPERATION_TYPE_ENQUEUE,
        RINGBUFFER_OPERATION_TYPE_DEQUEUE,
        RINGBUFFER_OPERATION_TYPE_SYNC_HEAD,
        RINGBUFFER_OPERATION_TYPE_SYNC_TAIL,
        RINGBUFFER_OPERATION_TYPE_PEEK_TO_END,
    } RINGBUFFER_OPERATION_TYPE;

    typedef struct RingBuffer
    {
        void *data;
        uint32_t dataWidth;
        uint32_t size;
        uint32_t write; // 指向当前最新的数据
        uint32_t read;
        //uint8_t writeMirror;
        //uint8_t readMirror;
        union
        {
            uint8_t allStatus;
            struct
            {
                uint8_t overflowed : 1;
            } statusBits;
        };

        void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type);
        // union {
        //     uint8_t allFlags;
        //     struct
        //     {
        //         uint8_t allowCoverTail : 1;
        //     } flagBits;
        // };

    } RingBuffer;

    /**
     * @brief 
     * 
     * @param queue 
     * @param dataPtr 
     * @param maxSize 
     */
    void ringbuffer_create(RingBuffer *queue, void *data, uint32_t dataWidth, uint32_t maxSize);
    bool ringbuffer_is_full(RingBuffer *queue);
    bool ringbuffer_is_empty(RingBuffer *queue);
    bool ringbuffer_space_enough(RingBuffer *queue, uint32_t length);
    uint32_t ringbuffer_mem_size_get(RingBuffer *queue);
    uint32_t ringbuffer_count_get(RingBuffer *queue);
    void *ringbuffer_write_ptr_get(RingBuffer *queue);
    void *ringbuffer_read_ptr_get(RingBuffer *queue);
    OP_RESULT ringbuffer_write_index_sync(RingBuffer *queue, uint32_t newHead);
    OP_RESULT ringbuffer_read_index_sync(RingBuffer *queue, uint32_t newTail);
    OP_RESULT ringbuffer_read_offset_sync(RingBuffer *queue, uint32_t offset);
    OP_RESULT ringbuffer_write(RingBuffer *queue, void *valuePtr, uint32_t length, uint8_t allowCoverTail, uint32_t *actualLength);
    OP_RESULT ringbuffer_read(RingBuffer *queue, void *valuePtr, uint32_t length, uint32_t *actualLength);
    OP_RESULT ringbuffer_peek(RingBuffer *queue, void **data);
    OP_RESULT ringbuffer_index_peek(RingBuffer *queue, uint32_t index, void **data);
    OP_RESULT ringbuffer_offset_peek(RingBuffer *queue, uint32_t offset, void **data);
    void ringbuffer_operation_notify_register(RingBuffer *queue, void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type));

    void *ringbuffer_index_peek_directly(RingBuffer *queue, uint32_t index);
    void *ringbuffer_offset_peek_directly(RingBuffer *queue, uint32_t offset);
    uint32_t ringbuffer_offset_to_index_convert(RingBuffer *queue, uint32_t offset);
    uint32_t ringbuffer_index_wrap(RingBuffer *queue, uint32_t index);

    // typedef struct RingBuffer32
    // {
    //     uint32_t *data;
    //     uint32_t size;
    //     uint32_t write; // 指向当前最新的数据
    //     uint32_t read;
    //     // uint8_t writeMirror;
    //     // uint8_t readMirror;
    //     union
    //     {
    //         uint8_t allStatus;
    //         struct
    //         {
    //             uint8_t overflowed : 1;
    //         } statusBits;
    //     };

    //     void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type);
    //     // union {
    //     //     uint8_t allFlags;
    //     //     struct
    //     //     {
    //     //         uint8_t allowCoverTail : 1;
    //     //     } flagBits;
    //     // };

    // } RingBuffer32;

    // /**
    //  * @brief
    //  *
    //  * @param queue
    //  * @param dataPtr
    //  * @param maxSize
    //  */
    // void ringbuffer32_create(RingBuffer32 *queue, uint32_t *dataPtr, uint32_t maxSize);
    // uint8_t ringbuffer32_is_full(RingBuffer32 *queue);
    // uint8_t ringbuffer32_is_empty(RingBuffer32 *queue);
    // uint8_t ringbuffer32_space_enough(RingBuffer32 *queue, uint32_t length);
    // uint32_t ringbuffer32_mem_size_get(RingBuffer32 *queue);
    // uint32_t ringbuffer32_count_get(RingBuffer32 *queue);
    // uint8_t *ringbuffer32_write_ptr_get(RingBuffer32 *queue);
    // uint8_t *ringbuffer32_read_ptr_get(RingBuffer32 *queue);
    // uint8_t ringbuffer32_write_index_sync(RingBuffer32 *queue, uint32_t newHead);
    // uint8_t ringbuffer32_read_index_sync(RingBuffer32 *queue, uint32_t newTail);
    // int32_t ringbuffer32_write(RingBuffer32 *queue, uint32_t *valuePtr, uint32_t length, uint8_t allowCoverTail);
    // int32_t ringbuffer32_read(RingBuffer32 *queue, uint32_t *valuePtr, uint32_t length);
    // uint8_t ringbuffer32_peek_to_end(RingBuffer32 *queue, uint32_t **valuePtr, uint32_t *length);

    // void ringbuffer32_operation_notify_register(RingBuffer32 *queue, void (*OperationNotify)(RINGBUFFER_OPERATION_TYPE type));

#ifdef __cplusplus
}
#endif

#endif // __RINGBUFFER_H__