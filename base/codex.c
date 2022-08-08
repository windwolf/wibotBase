#include "codex.h"
/**
 * @brief  Convert a 2 digit decimal to BCD format.
 * @param  Value Byte to be converted
 * @retval Converted byte
 */
uint8_t byte2bcd(uint8_t Value)
{
    uint32_t bcdhigh = 0U;
    uint8_t Param = Value;

    while (Param >= 10U)
    {
        bcdhigh++;
        Param -= 10U;
    }

    return ((uint8_t)(bcdhigh << 4U) | Param);
}

/**
 * @brief  Convert from 2 digit BCD to Binary.
 * @param  Value BCD value to be converted
 * @retval Converted word
 */
uint8_t bcd2byte(uint8_t Value)
{
    uint32_t tmp;
    tmp = (((uint32_t)Value & 0xF0U) >> 4U) * 10U;
    return (uint8_t)(tmp + ((uint32_t)Value & 0x0FU));
}