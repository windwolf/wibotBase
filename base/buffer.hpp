#ifndef __WWBASE_BUFFER_HPP__
#define __WWBASE_BUFFER_HPP__

#include "stdint.h"

namespace wibot
{

	struct Buffer8
	{
	 public:
		uint8_t* data;
		uint32_t size;

	 public:
		void clear()
		{
			for (uint32_t i = 0; i < size; i++)
			{
				data[i] = 0;
			}
		};
	};

#define BUFFER8_DECLARE(name, sz)                                                                  \
    uint8_t name##_##data[sz];                                                                     \
    BUFFER8 name = {.data = name##_##data, .size = sz};

#define BUFFER8_DECLARE_STATIC(name, sz)                                                           \
    static uint8_t name##_##data[sz];                                                              \
    static Buffer8 name = {.data = name##_##data, .size = sz};
#define BUFFER8_DATA_REF(name) (name##_##data)

	struct Buffer16
	{
	 public:
		uint16_t* data;
		uint32_t size;

	 public:
		void clear()
		{
			for (uint32_t i = 0; i < size; i++)
			{
				data[i] = 0;
			}
		};
	};

#define BUFFER16_DECLARE(name, sz)                                                                 \
    uint16_t name##_##data[sz];                                                                    \
    Buffer16 name = {.data = name##_##data, .size = sz};

#define BUFFER16_DECLARE_STATIC(name, sz)                                                          \
    static uint16_t name##_##data[sz];                                                             \
    static Buffer16 name = {.data = name##_##data, .size = sz};
#define BUFFER16_DATA_REF(name) (name##_##data)
// #define BUFFER16_SIZE(name) (name.size)

	struct Buffer32
	{
	 public:
		uint32_t* data;
		uint32_t size;

	 public:
		void clear()
		{
			for (uint32_t i = 0; i < size; i++)
			{
				data[i] = 0;
			}
		};

	};

#define BUFFER32_DECLARE(name, sz)                                                                 \
    uint32_t name##_##data[sz];                                                                    \
    Buffer32 name = {.data = name##_##data, .size = sz};

#define BUFFER32_DECLARE_STATIC(name, sz)                                                          \
    static uint32_t name##_##data[sz];                                                             \
    static Buffer32 name = {.data = name##_##data, .size = sz};
#define BUFFER32_DATA_REF(name) (name##_##data)

} // namespace wibot

#endif // __WWBASE_BUFFER_HPP__
