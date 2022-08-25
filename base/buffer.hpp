#ifndef __WWBASE_BUFFER_HPP__
#define __WWBASE_BUFFER_HPP__

#include "stdint.h"

namespace ww {
struct Buffer {
  public:
    uint8_t *data;
    uint32_t size;
};

#define BUFFER_DECLARE(name, size)                                             \
    uint8_t name##_##data[size];                                               \
    Buffer name = {.data = name##_##data, .size = size};

#define BUFFER_DECLARE_STATIC(name, size)                                      \
    static uint8_t name##_##data[size];                                        \
    static Buffer name = {.data = name##_##data, .size = size};
#define BUFFER_DATA_REF(name) (name##_##data)

} // namespace ww

#endif // __WWBASE_BUFFER_HPP__