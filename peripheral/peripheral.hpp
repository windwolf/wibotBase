#ifndef __WW_PERIPHERAL_PERIPHERAL_HPP__
#define __WW_PERIPHERAL_PERIPHERAL_HPP__

#include "buffer.hpp"
#include "peripheral_config.hpp"
#include "config_check.hpp"
#include "perip_port.hpp"

#include "stdbool.h"
#include "stdint.h"
#include "wait_handler.hpp"
#ifndef MAX_PERIPHERAL_COUNT
#define MAX_PERIPHERAL_COUNT 20
#endif  // MAX_PERIPHERAL_COUNT

namespace wibot::peripheral {

class Peripherals {
   public:
    struct PeripheralItem {
        const char* peripheralName;
        /**
         * @brief  Pointer of wibot peripheral
         */
        void*       peripheral;
        /**
         * @brief  Pointer of HAL port instance
         */
        void*       instance;
    };

    static Result register_peripheral(const char* peripheralName, void* peripheral, void* instance);
    static void   unregister_peripheral(const char* peripheralName, void* peripheral);
    static void*  get_peripheral(void* instance);

   private:
    static PeripheralItem _peripherals[MAX_PERIPHERAL_COUNT];
    // uint32_t peripheralCount;
};

}  // namespace wibot::peripheral

#endif  // __WW_PERIPHERAL_PERIPHERAL_HPP__
