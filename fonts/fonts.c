#include "fonts/fonts.h"
#include "string.h"

static inline void setPixel(uint8_t *buffer, uint32_t memOffset, uint32_t pixelSize, Color color)
{
    if (pixelSize == PIXEL_SIZE_8BIT)
    {
        buffer[memOffset] = (uint8_t)(color.Value);
    }
    else if (pixelSize == PIXEL_SIZE_16BIT)
    {
        ((uint16_t *)buffer)[memOffset] = (uint16_t)(color.Value);
    }
    else if (pixelSize == PIXEL_SIZE_32BIT)
    {
        ((uint32_t *)buffer)[memOffset] = (uint32_t)(color.Value);
    }
    else if (pixelSize == PIXEL_SIZE_24BIT)
    {
        buffer[memOffset * 3] = (uint8_t)(color.Value >> 16);
        buffer[memOffset * 3 + 1] = (uint8_t)(color.Value >> 8);
        buffer[memOffset * 3 + 2] = (uint8_t)(color.Value);
    }
    else if (pixelSize == PIXEL_SIZE_1BIT)
    {
        if (color.Value == 1)
        {
            ((uint8_t *)buffer)[memOffset / 8] |= (uint8_t)(color.Value) << (7 - (memOffset % 8));
        }
        else
        {
            ((uint8_t *)buffer)[memOffset / 8] &= ~((uint8_t)(color.Value) << (7 - (memOffset % 8)));
        }
    }
}

/**
 * @brief whole font size in bytes
 *
 * @param fontInfo
 * @return uint8_t
 */
static inline uint8_t getFontDataSize(FontInfo *fontInfo)
{
    if (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
    {
        return (fontInfo->width + 7) / 8 * fontInfo->height;
    }
    else
    {
        return (fontInfo->height + 7) / 8 * fontInfo->width;
    }
}

/**
 * @brief font data size in one row in bytes
 *
 * @param fontInfo
 * @return uint8_t
 */
static inline uint8_t getFontRowSize(FontInfo *fontInfo)
{
    if (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
    {
        return (fontInfo->width + 7) / 8;
    }
    else
    {
        return (fontInfo->height + 7) / 8;
    }
}

bool FONTS_CalcSpace(uint16_t x, uint16_t y, char *str, FontInfo *fontInfo, uint8_t spacing, SpaceInfo *spaceInfo)
{
    uint32_t len = strlen(str);
    if (len == 0)
    {
        spaceInfo->x = x;
        spaceInfo->y = y;
        spaceInfo->width = 0;
        spaceInfo->height = 0;
        return true;
    }
    else
    {
        spaceInfo->x = x;
        spaceInfo->y = y;
        spaceInfo->width = len * (fontInfo->width + spacing) - 1;
        spaceInfo->height = fontInfo->height;
        return true;
    }
}

bool FONTS_FillData(uint8_t *buffer, CanvasInfo *canvas, uint16_t x, uint16_t y, char *str, FontInfo *fontInfo, FontDrawInfo *fontDrawInfo)
{
    // if (canvas->direction != fontInfo->direction)
    // {
    //     return false;
    // }
    uint16_t len = strlen(str);
    if (len == 0)
    {
        return true;
    }
    else
    {
        if ((y + fontInfo->height) > canvas->height)
        {
            return false;
        }
        uint8_t fontMemSize = (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
                                  ? (fontInfo->width + 7) / 8 * fontInfo->height
                                  : (fontInfo->height + 7) / 8 * fontInfo->width;
        uint8_t fontMemRowSize = (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
                                     ? (fontInfo->width + 7) / 8
                                     : (fontInfo->height + 7) / 8;

        uint8_t fontMemWidth = (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
                                   ? fontInfo->width
                                   : fontInfo->height;
        uint8_t fontMemHeight = (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
                                    ? fontInfo->height
                                    : fontInfo->width;

        uint8_t lineSpacingMemWidth = (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
                                          ? fontDrawInfo->lineSpacing
                                          : fontInfo->height;
        uint8_t lineSpacingMemHeight = (fontInfo->direction == FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL)
                                           ? fontInfo->height
                                           : fontDrawInfo->lineSpacing;

        uint32_t pixelSize = canvas->pixelSize;
        uint8_t memX = canvas->direction == CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL ? x : y;
        uint8_t memY = canvas->direction == CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL ? y : x;
        uint8_t canvasMemWidth = canvas->direction == CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL ? canvas->width : canvas->height;
        // uint8_t canvasMemHeight = canvas->direction == CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL ? canvas->height : canvas->width;
        for (uint16_t i = 0; i < len; i++)
        {
            // draw spacing;
            if (i != 0)
            {
                if ((x + fontDrawInfo->spacing) >= canvas->width)
                {
                    return false;
                }
                for (uint16_t j = 0; j < lineSpacingMemHeight; j++)
                {
                    uint32_t y_index = memY + j;
                    for (uint16_t k = 0; k < lineSpacingMemWidth; k++)
                    {
                        uint32_t x_index = memX + k;
                        uint32_t memOffset = y_index * canvasMemWidth + x_index;
                        setPixel(buffer, memOffset, pixelSize, fontDrawInfo->backColor);
                    }
                }
                x += fontDrawInfo->spacing;
            }

            // draw charactor;
            if ((x + fontInfo->width) > canvas->width)
            {
                return false;
            }
            memX = canvas->direction == CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL ? x : y;
            memY = canvas->direction == CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL ? y : x;

            const uint8_t *fontData = &(fontInfo->table[(str[i] - 32) * fontMemSize]);

            for (uint16_t j = 0; j < fontMemHeight; j++)
            {
                uint32_t y_index = memY + j;
                for (uint16_t k = 0; k < fontMemWidth; k++)
                {
                    uint32_t x_index = memX + k;
                    uint32_t offset = y_index * canvasMemWidth + x_index;
                    Color color;
                    if (fontData[j * fontMemRowSize + k / 8] & (1 << (fontMemWidth - k - 1)))
                    {
                        color = fontDrawInfo->foreColor;
                    }
                    else
                    {
                        color = fontDrawInfo->backColor;
                    }

                    setPixel(buffer, offset, pixelSize, color);
                }
            }
            x += fontInfo->width;
        }
        return true;
    }
}
