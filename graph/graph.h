#ifndef __WWBASE_GRAPH_GRAPH_H__
#define __WWBASE_GRAPH_GRAPH_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"

    typedef union Color
    {
        uint32_t Value;
        struct
        {
            uint8_t a;
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } ARGB;
    } Color;

#define COLOR_WHITE ((Color)0x00FFFFFFUL)
#define COLOR_SILVER ((Color)0x00C0C0C0UL)
#define COLOR_GRAY ((Color)0x00808080UL)
#define COLOR_BLACK ((Color)0x00000000UL)
#define COLOR_RED ((Color)0x00FF0000UL)
#define COLOR_MAROON ((Color)0x00800000UL)
#define COLOR_YELLOW ((Color)0x00FFFF00UL)
#define COLOR_OLIVE ((Color)0x00808000UL)
#define COLOR_LIME ((Color)0x0000FF00UL)
#define COLOR_GREEN ((Color)0x00008000UL)
#define COLOR_AQUA ((Color)0x0000FFFFUL)
#define COLOR_TEAL ((Color)0x00008080UL)
#define COLOR_BLUE ((Color)0x000000FFUL)
#define COLOR_NAVY ((Color)0x00000080UL)
#define COLOR_FUCHSIA ((Color)0x00FF00FFUL)
#define COLOR_PURPLE ((Color)0x00800080UL)

#ifdef __cplusplus
}
#endif

#endif // __WWBASE_GRAPH_GRAPH_H__