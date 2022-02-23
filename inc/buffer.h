#ifndef ___BUFFER_H__
#define ___BUFFER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

    typedef struct Buffer
    {
        void *data;
        uint32_t size;
        //uint32_t count;
    } Buffer;

    // void Buffer_Init(Buffer *buffer, void *data, uint32_t size);
    // void Buffer_SetCount8(Buffer *buffer, uint32_t count);

#define BUFFER_DECLARE(name, _size) \
    uint8_t name##_##data[_size];   \
    Buffer name = {.data = name##_##data, .size = _size};

#define BUFFER_DECLARE_STATIC(name, _size) \
    static uint8_t name##_##data[_size];   \
    static Buffer name = {.data = name##_##data, .size = _size};

#ifdef __cplusplus
}
#endif

#endif // ___BUFFER_H__