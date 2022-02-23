#ifndef __BASIC_RING_BUFFER8_H__
#define __BASIC_RING_BUFFER8_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
    typedef enum
    {
        CIRCLE_BUFFER_OPERATION_ENQUEUE,
        CIRCLE_BUFFER_OPERATION_DEQUEUE,
        CIRCLE_BUFFER_OPERATION_SYNC_HEAD,
        CIRCLE_BUFFER_OPERATION_SYNC_TAIL,
        CIRCLE_BUFFER_OPERATION_PEEK_TO_END,
    } RingBuffer8_OperationType;

    typedef struct
    {
        uint8_t *data;
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

        void (*OperationNotify)(RingBuffer8_OperationType type);
        // union {
        //     uint8_t allFlags;
        //     struct
        //     {
        //         uint8_t allowCoverTail : 1;
        //     } flagBits;
        // };

    } RingBuffer8;

    /**
     * @brief 
     * 
     * @param queue 
     * @param dataPtr 
     * @param maxSize 
     */
    void RingBuffer8_Create(RingBuffer8 *queue, uint8_t *dataPtr, uint32_t maxSize);
    uint8_t RingBuffer8_IsFull(RingBuffer8 *queue);
    uint8_t RingBuffer8_IsEmpty(RingBuffer8 *queue);
    uint8_t RingBuffer8_IsEnoughSpace(RingBuffer8 *queue, uint32_t length);
    uint32_t RingBuffer8_GetMemorySize(RingBuffer8 *queue);
    uint32_t RingBuffer8_GetCount(RingBuffer8 *queue);
    uint8_t *RingBuffer8_GetWrite(RingBuffer8 *queue);
    uint8_t *RingBuffer8_GetRead(RingBuffer8 *queue);
    uint8_t RingBuffer8_SyncWrite(RingBuffer8 *queue, uint32_t newHead);
    uint8_t RingBuffer8_SyncRead(RingBuffer8 *queue, uint32_t newTail);
    int32_t RingBuffer8_Write(RingBuffer8 *queue, uint8_t *valuePtr, uint32_t length, uint8_t allowCoverTail);
    int32_t RingBuffer8_Read(RingBuffer8 *queue, uint8_t *valuePtr, uint32_t length);
    uint8_t RingBuffer8_PeekToEnd(RingBuffer8 *queue, uint8_t **valuePtr, uint32_t *length);

    void RingBuffer8_RegisterOperationNotify(RingBuffer8 *queue, void (*OperationNotify)(RingBuffer8_OperationType type));

#ifdef __cplusplus
}
#endif

#endif // __BASIC_RING_BUFFER8_H__