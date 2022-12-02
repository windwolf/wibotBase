#ifndef __WWBASE_GRAPH_GRAPH_HPP__
#define __WWBASE_GRAPH_GRAPH_HPP__

#include "base.hpp"

namespace wibot::graph
{
union Color {
    uint32_t value;
    struct
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
};

struct Color888
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

union Color565 {
    uint16_t value;
    struct
    {
        uint8_t b : 5;
        uint8_t g : 6;
        uint8_t r : 5;
    };
};

} // namespace wibot::graph

#define COLOR_WHITE (Color{.value=0x00FFFFFFUL})
#define COLOR_SILVER (Color{.value=0x00C0C0C0UL})
#define COLOR_GRAY (Color{.value=0x00808080UL})
#define COLOR_BLACK (Color{.value=0x00000000UL})
#define COLOR_RED (Color{.value=0x00FF0000UL})
#define COLOR_MAROON (Color{.value=0x00800000UL})
#define COLOR_YELLOW (Color{.value=0x00FFFF00UL})
#define COLOR_OLIVE (Color{.value=0x00808000UL})
#define COLOR_LIME (Color{.value=0x0000FF00UL})
#define COLOR_GREEN (Color{.value=0x00008000UL})
#define COLOR_AQUA (Color{.value=0x0000FFFFUL})
#define COLOR_TEAL (Color{.value=0x00008080UL})
#define COLOR_BLUE (Color{.value=0x000000FFUL})
#define COLOR_NAVY (Color{.value=0x00000080UL})
#define COLOR_FUCHSIA (Color{.value=0x00FF00FFUL})
#define COLOR_PURPLE (Color{.value=0x00800080UL})

#endif // __WWBASE_GRAPH_GRAPH_HPP__
