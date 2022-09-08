#ifndef ___BASE_HPP__
#define ___BASE_HPP__

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <type_traits>

#define ALIGN(n) __attribute__((aligned(n)))
#define ALIGN32 __attribute__((aligned(32)))
#define PACKED __attribute__((__packed__))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define min(a, b) (((a) <= (b)) ? (a) : (b))
#define max(a, b) (((a) >= (b)) ? (a) : (b))

#define TIMEOUT_NOWAIT 0x00000000
#define TIMEOUT_FOREVER 0xFFFFFFFF

template <typename E> constexpr auto to_underlying(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

namespace ww
{

enum class Result : uint32_t
{
    OK = 0,
    GeneralError = (uint32_t)1, // 无法归类的一场
    Busy = (uint32_t)2,
    Timeout = (uint32_t)3,
    NoResource = (uint32_t)4,       // 没有资源(事件,外设等)
    InvalidParameter = (uint32_t)5, ///< Parameter error.
    NoMemory = (uint32_t)6,
    InvalidOperationFromISR = (uint32_t)7,

    NotSupport = (uint32_t)8,
    StatusReserved = 0x7FFFFFFF ///< Prevents enum down-size compiler optimization.
};

enum class DataWidth : uint8_t
{
    Bit8 = 0x00,
    Bit16 = 0x01,
    Bit24 = 0x02,
    Bit32 = 0x03,
};

char *strtrim(char *str, char delimit);

uint32_t fast_log2(uint32_t _val);

class Initializable
{
  public:
    Result init();
    void deinit();

  protected:
    virtual Result _init() = 0;
    virtual void _deinit() = 0;

  private:
    struct
    {
        Result initErrorCode;
        bool inited : 1;
    } initState;
};

#define INIT_BEGIN() Result rst;
#define INIT_END() return rst;

#define BASE_INIT_ERROR_CHECK(className)                                                           \
    rst = className::init();                                                                       \
    if (rst != Result::OK)                                                                         \
    {                                                                                              \
        return rst;                                                                                \
    }
#define MEMBER_INIT_ERROR_CHECK(instance)                                                          \
    rst = instance.init();                                                                         \
    if (rst != Result::OK)                                                                         \
    {                                                                                              \
        return rst;                                                                                \
    }
#define PTR_INIT_ERROR_CHECK(instance)                                                             \
    rst = instance->init();                                                                        \
    if (rst != Result::OK)                                                                         \
    {                                                                                              \
        return rst;                                                                                \
    }

#define BASE_DEINIT(className) className::deinit();
#define MEMBER_DEINIT(instance) instance.deinit();
#define PTR_DEINIT(instance) instance->deinit();
} // namespace ww

#endif // ___BASE_HPP__