#ifndef ___BASE_HPP__
#define ___BASE_HPP__

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define ALIGN(n) __attribute__((aligned(n)))
#define ALIGN32 __attribute__((aligned(32)))
#define PACKED __attribute__((__packed__))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define min(a, b) (((a) <= (b)) ? (a) : (b))
#define max(a, b) (((a) >= (b)) ? (a) : (b))

#define TIMEOUT_NOWAIT 0x00000000
#define TIMEOUT_FOREVER 0xFFFFFFFF

namespace ww
{

enum Result : uint32_t
{
    Result_OK = 0,
    Result_GeneralError = (uint32_t)1, // 无法归类的一场
    Result_Busy = (uint32_t)2,
    Result_Timeout = (uint32_t)3,
    Result_NoResource = (uint32_t)4,       // 没有资源(事件,外设等)
    Result_InvalidParameter = (uint32_t)5, ///< Parameter error.
    Result_NoMemory = (uint32_t)6,
    Result_InvalidOperationFromISR = (uint32_t)7,

    Result_NotSupport = (uint32_t)8,
    Result_StatusReserved = 0x7FFFFFFF ///< Prevents enum down-size compiler optimization.
};

enum DataWidth
{
    DATAWIDTH_8 = 0x00,
    DATAWIDTH_16 = 0x01,
    DATAWIDTH_24 = 0x02,
    DATAWIDTH_32 = 0x03,
};

char *strtrim(char *str, char delimit);

uint32_t fast_log2(uint32_t _val);

class Initializable
{
  public:
    Result initErrorCode;
};

#define BASE_INIT_ERROR_CHECK()                                                                    \
    if (initErrorCode != Result_OK)                                                                \
    {                                                                                              \
        return;                                                                                    \
    }
#define MEMBER_INIT_ERROR_CHECK(instance)                                                          \
    initErrorCode = instance.initErrorCode;                                                        \
    if (initErrorCode != Result_OK)                                                                \
    {                                                                                              \
        return;                                                                                    \
    }
#define PTR_INIT_ERROR_CHECK(instance)                                                             \
    initErrorCode = instance->initErrorCode;                                                       \
    if (initErrorCode != Result_OK)                                                                \
    {                                                                                              \
        return;                                                                                    \
    }
} // namespace ww

#endif // ___BASE_HPP__