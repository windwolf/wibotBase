#ifndef __WWBASE_GRAPH_FONTS_HPP_
#define __WWBASE_GRAPH_FONTS_HPP_

#include "graph.hpp"

namespace ww::graph
{

enum FONT_MEMORY_LAYOUT_DIRECTION
{
    FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL = 0,
    FONT_MEMORY_LAYOUT_DIRECTION_VERTICAL,
};

struct FontInfo
{
    const uint8_t *table;
    uint16_t width;
    uint16_t height;
    FONT_MEMORY_LAYOUT_DIRECTION direction;
};

extern FontInfo Font17x24;

extern FontInfo Font14x20;

extern FontInfo Font11x16;

extern FontInfo Font7x12;

extern FontInfo Font5x8;

extern FontInfo Font6x8_v;

extern FontInfo Font8x16_v;

struct SpaceInfo
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

struct FontDrawInfo
{
    Color foreColor;
    Color backColor;
    uint8_t spacing;
    uint8_t lineSpacing;
};

enum class PixelSize : uint32_t
{
    Bit1 = 0x00000001,
    Bit8 = 0x000000FF,
    Bit16 = 0x0000FFFF,
    Bit24 = 0x00FFFFFF,
    Bit32 = 0xFFFFFFFF,
};

enum class CanvasMemoryLayoutDirection
{
    Horizontal = 0,
    Vertical,
};

struct CanvasInfo
{
    uint16_t width;
    uint16_t height;
    PixelSize pixelSize;
    CanvasMemoryLayoutDirection direction;
};

bool FONTS_CalcSpace(uint16_t x, uint16_t y, char *str, FontInfo *fontInfo, uint8_t spacing,
                     SpaceInfo *spaceInfo);

bool FONTS_FillData(uint8_t *buffer, CanvasInfo *canvas, uint16_t x, uint16_t y, char *str,
                    FontInfo *fontInfo, FontDrawInfo *fontDrawInfo);
} // namespace ww::graph
#endif //__WWBASE_GRAPH_FONTS_HPP_
