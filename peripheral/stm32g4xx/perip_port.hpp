#ifndef __PERIP_PORT_HPP__
#define __PERIP_PORT_HPP__

#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_tim.h"

/********** I2C **************************/

#ifdef HAL_I2C_MODULE_ENABLED
#define I2C_PER_DECL
#define I2C_CTOR_ARG I2C_HandleTypeDef &handle
#define I2C_FIELD_DECL I2C_HandleTypeDef &_handle;
#define I2C_CALLBACK_ARG I2C_HandleTypeDef *handle
#else
#define I2C_PER_DECL
#define I2C_CTOR_ARG uint32_t dummy
#define I2C_FIELD_DECL uint32_t dummy;
#define I2C_CALLBACK_ARG uint32_t dummy
#endif // HAL_I2C_MODULE_ENABLED

/********** I2C **************************/

/********** SPI **************************/

#ifdef HAL_SPI_MODULE_ENABLED
#define SPI_PER_DECL
#define SPI_CTOR_ARG SPI_HandleTypeDef &handle
#define SPI_FIELD_DECL SPI_HandleTypeDef &_handle;
#define SPI_CALLBACK_ARG SPI_HandleTypeDef *handle
#else
#define SPI_PER_DECL
#define SPI_CTOR_ARG uint32_t dummy
#define SPI_FIELD_DECL uint32_t dummy;
#define SPI_CALLBACK_ARG uint32_t dummy
#endif // HAL_SPI_MODULE_ENABLED

/********** SPI **************************/

/********** PIN **************************/

#ifdef HAL_GPIO_MODULE_ENABLED
#define PIN_PER_DECL
#define PIN_CTOR_ARG GPIO_TypeDef &port
#define PIN_FIELD_DECL GPIO_TypeDef &_port;
#define PIN_CALLBACK_ARG GPIO_TypeDef *port
#else
#define PIN_PER_DECL
#define PIN_CTOR_ARG uint32_t dummy
#define PIN_FIELD_DECL uint32_t dummy;
#define PIN_CALLBACK_ARG uint32_t dummy
#endif // HAL_GPIO_MODULE_ENABLED

/********** PIN **************************/

/********** PWM **************************/

#ifdef HAL_TIM_MODULE_ENABLED
#define PWM_PER_DECL
#define PWM_CTOR_ARG TIM_HandleTypeDef &handle
#define PWM_FIELD_DECL TIM_HandleTypeDef &_handle;
#define PWM_CALLBACK_ARG TIM_HandleTypeDef *handle
#else
#define PWM_PER_DECL
#define PWM_CTOR_ARG uint32_t dummy
#define PWM_FIELD_DECL uint32_t dummy;
#define PWM_CALLBACK_ARG uint32_t dummy
#endif // HAL_TIM_MODULE_ENABLED

/********** PWM **************************/

/********** UART **************************/

#ifdef HAL_UART_MODULE_ENABLED
#define UART_PER_DECL
#define UART_CTOR_ARG UART_HandleTypeDef &handle
#define UART_FIELD_DECL UART_HandleTypeDef &_handle;
#define UART_CALLBACK_ARG UART_HandleTypeDef *handle
#else
#define UART_PER_DECL
#define UART_CTOR_ARG uint32_t dummy
#define UART_FIELD_DECL uint32_t dummy;
#define UART_CALLBACK_ARG uint32_t dummy
#endif // HAL_UART_MODULE_ENABLED

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

#endif // __PERIP_PORT_HPP__