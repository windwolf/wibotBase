#ifndef __WW_PERIPHERAL_PERIPHERAL_HPP__
#define __WW_PERIPHERAL_PERIPHERAL_HPP__

#include "config.h"
#include "stdbool.h"
#include "stdint.h"
#include "wait_handler.hpp"

namespace ww::peripheral
{

class Peripherals
{
  public:
    struct PeripheralItem
    {
        const char *peripheralName;
        void *peripheral;
        void *instance;
    };

    static void peripheral_register(const char *peripheralName,
                                    void *peripheral, void *instance);
    static void peripheral_unregister(const char *peripheralName,
                                      void *peripheral);
    static void *peripheral_get_by_instance(void *instance);

  private:
    static PeripheralItem _peripherals[MAX_PERIPHERAL_COUNT];
    // uint32_t peripheralCount;
};

} // namespace ww::peripheral

#endif // __WW_PERIPHERAL_PERIPHERAL_HPP__