#ifndef __WWBASE_BUFFER_HPP__
#define __WWBASE_BUFFER_HPP__

#include "base.hpp"
#include "arch.hpp"
#include "stdint.h"

namespace wibot {

struct Buffer8 {
   public:
    uint8_t* data;
    uint32_t size;

   public:
    void clear() {
        for (uint32_t i = 0; i < size; i++) {
            data[i] = 0;
        }
    };

    uint8_t getUint8(uint32_t index) const {
        return data[index];
    };
    void setUint8(uint32_t index, uint8_t value) {
        data[index] = value;
    };
    int8_t getInt8(uint32_t index) const {
        return *PTR_TO_INT8(data + index);
    };
    void setInt8(uint32_t index, int8_t value) {
        data[index] = value;
    };
    uint16_t getUint16(uint32_t index, bool littleEndian = true) const {
        auto v = arch::getUint16(data + index, littleEndian);
        return *PTR_TO_UINT16(&v);
    };
    void setUint16(uint32_t index, uint16_t value, bool littleEndian = true) {
        arch::setUint16(data + index, value, littleEndian);
    };
    int16_t getInt16(uint32_t index, bool littleEndian = true) const {
        auto v = arch::getUint16(data + index, littleEndian);
        return *PTR_TO_INT16(&v);
    };
    void setInt16(uint32_t index, int16_t value, bool littleEndian = true) {
        arch::setUint16(data + index, *PTR_TO_UINT16(&value), littleEndian);
    }

    uint32_t getUint32(uint32_t index, bool littleEndian = true) const {
        return arch::getUint32(data + index, littleEndian);
    };
    void setUint32(uint32_t index, uint32_t value, bool littleEndian = true) {
        arch::setUint32(data + index, value, littleEndian);
    };

    int32_t getInt32(uint32_t index, bool littleEndian = true) const {
        auto v = arch::getUint32(data + index, littleEndian);
        return *PTR_TO_INT32(&v);
    };
    void setInt32(uint32_t index, int32_t value, bool littleEndian = true) {
        arch::setUint32(data + index, *PTR_TO_UINT32(&value), littleEndian);
    };

    float getFloat(uint32_t index, bool littleEndian = true) const {
        auto v = arch::getUint32(data + index, littleEndian);
        return *PTR_TO_FLOAT(&v);
    };
    void setFloat(uint32_t index, float value, bool littleEndian = true) {
        arch::setUint32(data + index, *PTR_TO_UINT32(&value), littleEndian);
    };
};

#define BUFFER8_DECLARE(name, sz) \
    uint8_t name##_##data[sz];    \
    BUFFER8 name = {.data = name##_##data, .size = sz};

#define BUFFER8_DECLARE_STATIC(name, sz) \
    static uint8_t name##_##data[sz];    \
    static Buffer8 name = {.data = name##_##data, .size = sz};
#define BUFFER8_DATA_REF(name) (name##_##data)

struct Buffer16 {
   public:
    uint16_t* data;
    uint32_t  size;

   public:
    void clear() {
        for (uint32_t i = 0; i < size; i++) {
            data[i] = 0;
        }
    };
};

#define BUFFER16_DECLARE(name, sz) \
    uint16_t name##_##data[sz];    \
    Buffer16 name = {.data = name##_##data, .size = sz};

#define BUFFER16_DECLARE_STATIC(name, sz) \
    static uint16_t name##_##data[sz];    \
    static Buffer16 name = {.data = name##_##data, .size = sz};
#define BUFFER16_DATA_REF(name) (name##_##data)
// #define BUFFER16_SIZE(name) (name.size)

struct Buffer32 {
   public:
    uint32_t* data;
    uint32_t  size;

   public:
    void clear() {
        for (uint32_t i = 0; i < size; i++) {
            data[i] = 0;
        }
    };
};

#define BUFFER32_DECLARE(name, sz) \
    uint32_t name##_##data[sz];    \
    Buffer32 name = {.data = name##_##data, .size = sz};

#define BUFFER32_DECLARE_STATIC(name, sz) \
    static uint32_t name##_##data[sz];    \
    static Buffer32 name = {.data = name##_##data, .size = sz};
#define BUFFER32_DATA_REF(name) (name##_##data)

}  // namespace wibot

#endif  // __WWBASE_BUFFER_HPP__
