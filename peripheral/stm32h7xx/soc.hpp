#ifndef __WW_PERIPHERAL_SOC_HPP__
#define __WW_PERIPHERAL_SOC_HPP__

#include "i2c.hpp"
#include "misc.hpp"
#include "pin.hpp"
#include "spi.hpp"
#include "uart.hpp"
#include "pwm.hpp"

#include "stm32h7xx_hal.h"

#define CACHE_LINE_SIZE 4

#define DMA_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))

#define AXI_BUFFER __attribute__((section(".AXI_RAM1.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM1_BUFFER __attribute__((section(".RAM1.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM2_BUFFER __attribute__((section(".RAM2.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM3_BUFFER __attribute__((section(".RAM3.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM4_BUFFER __attribute__((section(".RAM4.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define BACKUP_BUFFER __attribute__((section(".Backup_RAM1.bss"))) __attribute__((aligned(CACHE_LINE_SIZE)))

#define AXI_DATA __attribute__((section(".AXI_RAM1.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM1_DATA __attribute__((section(".RAM1.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM2_DATA __attribute__((section(".RAM2.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM3_DATA __attribute__((section(".RAM3.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define RAM4_DATA __attribute__((section(".RAM4.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))
#define BACKUP_DATA __attribute__((section(".Backup_RAM1.data"))) __attribute__((aligned(CACHE_LINE_SIZE)))


#endif // __WW_PERIPHERAL_SOC_HPP__