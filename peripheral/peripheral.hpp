#ifndef __WW_PERIPHERAL_PERIPHERAL_HPP__
#define __WW_PERIPHERAL_PERIPHERAL_HPP__

#include "stdbool.h"
#include "stdint.h"
#include "perip_port.hpp"
#include "wait_handler.hpp"
#include "buffer.hpp"

#include "peripheral_config.hpp"
#include "config_check.hpp"
#ifndef MAX_PERIPHERAL_COUNT
#define MAX_PERIPHERAL_COUNT 20
#endif // MAX_PERIPHERAL_COUNT

namespace wibot::peripheral
{

	class Peripherals
	{
	 public:
		struct PeripheralItem
		{
			const char* peripheralName;
			void* peripheral;
			void* instance;
		};

		static void peripheral_register(const char* peripheralName, void* peripheral, void* instance);
		static void peripheral_unregister(const char* peripheralName, void* peripheral);
		static void* peripheral_get_by_instance(void* instance);

	 private:
		static PeripheralItem _peripherals[MAX_PERIPHERAL_COUNT];
		// uint32_t peripheralCount;
	};

} // namespace wibot::peripheral

#endif // __WW_PERIPHERAL_PERIPHERAL_HPP__
