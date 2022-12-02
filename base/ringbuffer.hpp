#ifndef __WWBASE_RINGBUFFER_HPP__
#define __WWBASE_RINGBUFFER_HPP__

#include "base.hpp"
#include "stdint.h"

namespace wibot
{
	enum class RingBufferOperationType
	{
		Enqueue,
		Dequeue,
		SyncHead,
		SyncTail,
		PeekToEnd,
	};

	class RingBuffer
	{
	 public:
		typedef void (* OperationNotify)(RingBufferOperationType type);

		RingBuffer(void* data, uint32_t dataWidth, uint32_t maxSize);
		bool is_full();
		bool is_empty();
		bool is_space_enough(uint32_t length);
		uint32_t mem_size_get();
		uint32_t count_get();
		void* write_ptr_get();
		void* read_ptr_get();
		Result write_index_sync(uint32_t newWrite);
		Result read_index_sync(uint32_t newRead);
		Result read_offset_sync(uint32_t offset);
		Result write(void* valuePtr, uint32_t length, uint8_t allowCoverTail, uint32_t* actualLength);
		Result write_fill(uint8_t* value, uint32_t length, uint8_t allowCoverTail,
			uint32_t* actualLength);
		Result read(void* valuePtr, uint32_t length, uint32_t& actualLength);
		Result peek(void*& data);
		Result index_peek(uint32_t index, void*& data);
		Result offset_peek(uint32_t offset, void*& data);

		void* index_peek_directly(uint32_t index);
		void* offset_peek_directly(uint32_t offset);
		uint32_t offset_to_index_convert(uint32_t offset);
		uint32_t index_wrap(int32_t index);

		void operation_notify_register(OperationNotify operationNotify);

		void* data_ptr_get();

	 private:
		void* _data;
		uint32_t _dataWidth;
		uint32_t _size;
		uint32_t _write; // Points to the next available slot to write to.
		uint32_t _read;
		union
		{
			uint8_t value;
			struct
			{
				uint8_t overflowed: 1;
			};
		} _status;
		OperationNotify _operationNotify;
	};
} // namespace wibot

#endif // __WWBASE_RINGBUFFER_HPP__
