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

#define TIMEOUT_NOWAIT 0x00000000
#define TIMEOUT_FOREVER 0xFFFFFFFF

#define _PI 3.14159265358979323846f
#define _2PI 6.28318530717958647692f
#define _PI_2 1.57079632679489661923f
#define _PI_3 1.04719755119659774615f
#define _2PI_3 2.09439510239319549231f
#define _4PI_3 4.18879020478639098462f
#define _5PI_3 5.23598775598298873078f
#define _SQRT3 1.73205080756887729352f
#define _SQRT3_2 0.86602540378443864676f
#define _1_SQRT3 0.57735026918962576450f

template<typename E>
constexpr auto to_underlying(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}

using namespace std;
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

char* strtrim(char* str, char delimit);

uint32_t fast_log2(uint32_t _val);

// -----------------
// Initializable

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
		bool inited: 1;
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


// -------------------------------
// Vector2, Vector3, Vector4

template<typename T>
struct Vector2
{
	T v1;
	T v2;

	Vector2() : v1(0), v2(0)
	{
	}
	Vector2(T v1, T v2) : v1(v1), v2(v2)
	{
	}

	Vector2<T> operator+(const Vector2<T>& other) const
	{
		Vector2<T> result;
		result.v1 = v1 + other.v1;
		result.v2 = v2 + other.v2;
		return result;
	};
	Vector2<T> operator+(const T other) const
	{
		Vector2<T> result;
		result.v1 = v1 + other;
		result.v2 = v2 + other;
		return result;
	};
	void operator+=(const Vector2<T>& other)
	{
		v1 += other.v1;
		v2 += other.v2;
	};
	void operator+=(const T other)
	{
		v1 += other;
		v2 += other;
	};

	Vector2<T> operator-(const Vector2<T>& other) const
	{
		Vector2<T> result;
		result.v1 = v1 - other.v1;
		result.v2 = v2 - other.v2;
		return result;
	};
	Vector2<T> operator-(const T& other) const
	{
		Vector2<T> result;
		result.v1 = v1 - other;
		result.v2 = v2 - other;
		return result;
	};
	void operator-=(const Vector2<T>& other)
	{
		v1 -= other.v1;
		v2 -= other.v2;
	};
	void operator-=(const T other)
	{
		v1 -= other;
		v2 -= other;
	};

	Vector2<T> operator*(const Vector2<T>& other) const
	{
		Vector2<T> result;
		result.v1 = v1 * other.v1;
		result.v2 = v2 * other.v2;
		return result;
	};
	Vector2<T> operator*(const T other) const
	{
		Vector2<T> result;
		result.v1 = v1 * other;
		result.v2 = v2 * other;
		return result;
	};
	void operator*=(const Vector2<T>& other)
	{
		v1 *= other.v1;
		v2 *= other.v2;
	};
	void operator*=(const T other)
	{
		v1 *= other;
		v2 *= other;
	};

	Vector2<T> operator/(const Vector2<T>& other) const
	{
		Vector2<T> result;
		result.v1 = v1 / other.v1;
		result.v2 = v2 / other.v2;
		return result;
	};
	Vector2<T> operator/(const T other) const
	{
		Vector2<T> result;
		result.v1 = v1 / other;
		result.v2 = v2 / other;
		return result;
	};
	void operator/=(const Vector2<T>& other)
	{
		v1 /= other.v1;
		v2 /= other.v2;
	};
	void operator/=(const T other)
	{
		v1 /= other;
		v2 /= other;
	};
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<uint32_t>;

template<typename T>
struct Vector3
{
	T v1;
	T v2;
	T v3;

	Vector3() : v1(0), v2(0), v3(0)
	{
	};
	Vector3(T v1, T v2, T v3) : v1(v1), v2(v2), v3(v3)
	{
	};

	Vector3<T> operator+(const Vector3<T>& other) const
	{
		Vector3<T> result;
		result.v1 = v1 + other.v1;
		result.v2 = v2 + other.v2;
		result.v3 = v3 + other.v3;
		return result;
	};
	Vector3<T> operator+(const T other) const
	{
		Vector3<T> result;
		result.v1 = v1 + other;
		result.v2 = v2 + other;
		result.v3 = v3 + other;
		return result;
	};
	void operator+=(const Vector3<T>& other)
	{
		v1 += other.v1;
		v2 += other.v2;
		v3 += other.v3;
	};
	void operator+=(const T other)
	{
		v1 += other;
		v2 += other;
		v3 += other;
	};

	Vector3<T> operator-(const Vector3<T>& other) const
	{
		Vector3<T> result;
		result.v1 = v1 - other.v1;
		result.v2 = v2 - other.v2;
		result.v3 = v3 - other.v3;
		return result;
	};
	Vector3<T> operator-(const T& other) const
	{
		Vector3<T> result;
		result.v1 = v1 - other;
		result.v2 = v2 - other;
		result.v3 = v3 - other;
		return result;
	};
	void operator-=(const Vector3<T>& other)
	{
		v1 -= other.v1;
		v2 -= other.v2;
		v3 -= other.v3;
	};
	void operator-=(const T other)
	{
		v1 -= other;
		v2 -= other;
		v3 -= other;
	};

	Vector3<T> operator*(const Vector3<T>& other) const
	{
		Vector2<T> result;
		result.v1 = v1 * other.v1;
		result.v2 = v2 * other.v2;
		result.v3 = v3 * other.v3;
		return result;
	};
	Vector3<T> operator*(const T other) const
	{
		Vector3<T> result;
		result.v1 = v1 * other;
		result.v2 = v2 * other;
		result.v3 = v3 * other;
		return result;
	};
	void operator*=(const Vector3<T>& other)
	{
		v1 *= other.v1;
		v2 *= other.v2;
		v3 *= other.v3;
	};
	void operator*=(const T other)
	{
		v1 *= other;
		v2 *= other;
		v3 *= other;
	};

	Vector3<T> operator/(const Vector3<T>& other) const
	{
		Vector3<T> result;
		result.v1 = v1 / other.v1;
		result.v2 = v2 / other.v2;
		result.v3 = v3 / other.v3;
		return result;
	};
	Vector3<T> operator/(const T other) const
	{
		Vector3<T> result;
		result.v1 = v1 / other;
		result.v2 = v2 / other;
		result.v3 = v3 / other;
		return result;
	};
	void operator/=(const Vector3<T>& other)
	{
		v1 /= other.v1;
		v2 /= other.v2;
		v3 /= other.v3;
	};
	void operator/=(const T other)
	{
		v1 /= other;
		v2 /= other;
		v3 /= other;
	};
};

using Vector3f = Vector3<float>;
using Vector3b = Vector3<uint8_t>;
using Vector3i = Vector3<uint32_t>;

template<typename T>
struct Vector4
{
	T v1;
	T v2;
	T v3;
	T v4;

	Vector4() : v1(0), v2(0), v3(0), v4(0)
	{
	};
	Vector4(T v1, T v2, T v3, T v4) : v1(v1), v2(v2), v3(v3), v4(v4)
	{
	};

	Vector4<T> operator+(const Vector4<T>& other) const
	{
		Vector4<T> result;
		result.v1 = v1 + other.v1;
		result.v2 = v2 + other.v2;
		result.v3 = v3 + other.v3;
		result.v4 = v4 + other.v4;
		return result;
	};
	Vector4<T> operator+(const T other) const
	{
		Vector4<T> result;
		result.v1 = v1 + other;
		result.v2 = v2 + other;
		result.v3 = v3 + other;
		result.v4 = v4 + other;
		return result;
	};
	void operator+=(const Vector4<T>& other)
	{
		v1 += other.v1;
		v2 += other.v2;
		v3 += other.v3;
		v4 += other.v4;
	};
	void operator+=(const T other)
	{
		v1 += other;
		v2 += other;
		v3 += other;
		v4 += other;
	};

	Vector4<T> operator-(const Vector4<T>& other) const
	{
		Vector4<T> result;
		result.v1 = v1 - other.v1;
		result.v2 = v2 - other.v2;
		result.v3 = v3 - other.v3;
		result.v4 = v4 - other.v4;
		return result;
	};
	Vector4<T> operator-(const T& other) const
	{
		Vector4<T> result;
		result.v1 = v1 - other;
		result.v2 = v2 - other;
		result.v3 = v3 - other;
		result.v4 = v4 - other;
		return result;
	};
	void operator-=(const Vector4<T>& other)
	{
		v1 -= other.v1;
		v2 -= other.v2;
		v3 -= other.v3;
		v4 -= other.v4;
	};
	void operator-=(const T other)
	{
		v1 -= other;
		v2 -= other;
		v3 -= other;
		v4 -= other;
	};

	Vector4<T> operator*(const Vector4<T>& other) const
	{
		Vector2<T> result;
		result.v1 = v1 * other.v1;
		result.v2 = v2 * other.v2;
		result.v3 = v3 * other.v3;
		result.v4 = v4 * other.v4;
		return result;
	};
	Vector4<T> operator*(const T other) const
	{
		Vector4<T> result;
		result.v1 = v1 * other;
		result.v2 = v2 * other;
		result.v3 = v3 * other;
		result.v4 = v4 * other;
		return result;
	};
	void operator*=(const Vector4<T>& other)
	{
		v1 *= other.v1;
		v2 *= other.v2;
		v3 *= other.v3;
		v4 *= other.v4;
	};
	void operator*=(const T other)
	{
		v1 *= other;
		v2 *= other;
		v3 *= other;
		v4 *= other;
	};

	Vector4<T> operator/(const Vector4<T>& other) const
	{
		Vector4<T> result;
		result.v1 = v1 / other.v1;
		result.v2 = v2 / other.v2;
		result.v3 = v3 / other.v3;
		result.v4 = v4 / other.v4;
		return result;
	};
	Vector4<T> operator/(const T other) const
	{
		Vector4<T> result;
		result.v1 = v1 / other;
		result.v2 = v2 / other;
		result.v3 = v3 / other;
		result.v4 = v4 / other;
		return result;
	};
	void operator/=(const Vector4<T>& other)
	{
		v1 /= other.v1;
		v2 /= other.v2;
		v3 /= other.v3;
		v4 /= other.v4;
	};
	void operator/=(const T other)
	{
		v1 /= other;
		v2 /= other;
		v3 /= other;
		v4 /= other;
	};
};

using Vector4f = Vector4<float>;
using Vector4b = Vector4<uint8_t>;
using Vector4i = Vector4<uint32_t>;

template<typename T>
class Configurable
{
 public:
	virtual void config_apply(T& config)
	{
		_config = config;
	}

 protected:
	T _config;
};

#endif // ___BASE_HPP__
