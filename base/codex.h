#ifndef __WW_BASE_CODEX_H__
#define __WW_BASE_CODEX_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

    uint8_t byte2bcd(uint8_t Value);

    uint8_t bcd2byte(uint8_t Value);
#ifdef __cplusplus
}
#endif

#endif // __WW_BASE_CODEX_H__