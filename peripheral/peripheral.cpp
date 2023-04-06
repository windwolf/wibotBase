
#include "peripheral.hpp"

#include "log.h"
LOGGER("peripheral")
namespace wibot::peripheral {

void Peripherals::register_peripheral(const char *peripheralName, void *peripheral,
                                      void *instance) {
    for (uint32_t i = 0; i < MAX_PERIPHERAL_COUNT; i++) {
        auto &periph = Peripherals::_peripherals[i];
        if (periph.peripheral == nullptr || periph.peripheral == peripheral) {
            periph.peripheral     = peripheral;
            periph.instance       = instance;
            periph.peripheralName = peripheralName;
            return;
        }
    }

    LOG_E("peripheral count overflow");
    return;
}

void Peripherals::unregister_peripheral(const char *peripheralName, void *peripheral) {
    for (uint32_t i = 0; i < MAX_PERIPHERAL_COUNT; i++) {
        if (Peripherals::_peripherals[i].peripheral == peripheral) {
            Peripherals::_peripherals[i].peripheral     = nullptr;
            Peripherals::_peripherals[i].instance       = nullptr;
            Peripherals::_peripherals[i].peripheralName = nullptr;
            return;
        }
    }
    LOG_E("peripheral not found");
    return;
}

void *Peripherals::get_peripheral(void *instance) {
    for (uint32_t i = 0; i < MAX_PERIPHERAL_COUNT; i++) {
        if (Peripherals::_peripherals[i].instance == instance) {
            return Peripherals::_peripherals[i].peripheral;
        }
    }

    LOG_E("peripheral not found");
    return nullptr;
}

Peripherals::PeripheralItem Peripherals::_peripherals[MAX_PERIPHERAL_COUNT] = {0};

}  // namespace wibot::peripheral
