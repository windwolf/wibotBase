#ifndef __WWBASE_GRAPH_FONTS_HPP_
#define __WWBASE_GRAPH_FONTS_HPP_

#include "graph.hpp"

namespace ww::graph {

enum FONT_MEMORY_LAYOUT_DIRECTION {
    FONT_MEMORY_LAYOUT_DIRECTION_HORIZONTAL = 0,
    FONT_MEMORY_LAYOUT_DIRECTION_VERTICAL,
};

struct FontInfo {
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

struct SpaceInfo {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};

struct FontDrawInfo {
    Color foreColor;
    Color backColor;
    uint8_t spacing;
    uint8_t lineSpacing;
};

enum PIXEL_SIZE {
    PIXEL_SIZE_1BIT = 0x00000001,
    PIXEL_SIZE_8BIT = 0x000000FF,
    PIXEL_SIZE_16BIT = 0x0000FFFF,
    PIXEL_SIZE_24BIT = 0x00FFFFFF,
    PIXEL_SIZE_32BIT = 0xFFFFFFFF,
};

enum CANVAS_MEMORY_LAYOUT_DIRECTION {
    CANVAS_MEMORY_LAYOUT_DIRECTION_HORIZONTAL = 0,
    CANVAS_MEMORY_LAYOUT_DIRECTION_VERTICAL,
};

struct CanvasInfo {
    uint16_t width;
    uint16_t height;
    PIXEL_SIZE pixelSize;
    CANVAS_MEMORY_LAYOUT_DIRECTION direction;
};

bool FONTS_CalcSpace(uint16_t x, uint16_t y, char *str, FontInfo *fontInfo,
                     uint8_t spacing, SpaceInfo *spaceInfo);

bool FONTS_FillData(uint8_t *buffer, CanvasInfo *canvas, uint16_t x, uint16_t y,
                    char *str, FontInfo *fontInfo, FontDrawInfo *fontDrawInfo);
} // namespace ww::graph
#endif //__WWBASE_GRAPH_FONTS_HPP_
