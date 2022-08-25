#ifndef __WW_PERIPHERAL_SOC_HPP__
#define __WW_PERIPHERAL_SOC_HPP__

#include "i2c.hpp"
#include "misc.hpp"
#include "pin.hpp"
#include "spi.hpp"
#include "uart.hpp"

#include "stm32f1xx_hal.h"

#define CACHE_LINE_SIZE 4

#define DMA_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))

#define AXI_BUFFER
#define RAM1_BUFFER
#define RAM2_BUFFER
#define RAM3_BUFFER
#define RAM4_BUFFER
#define BACKUP_BUFFER

#define AXI_DATA
#define RAM1_DATA
#define RAM2_DATA
#define RAM3_DATA
#define RAM4_DATA
#define BACKUP_DATA

#endif // __WW_PERIPHERAL_SOC_HPP__