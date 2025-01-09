#include <stdint.h>

typedef uint8_t color_value;

typedef uint32_t color_code;

typedef struct {
        color_value r;
        color_value g;
        color_value b;
} Color;

color_code rgb2cc(color_value r, color_value g, color_value b);
Color cc2Color(color_code color);
color_code Color2cc(Color color);
color_value get_r(color_code color);
color_value get_g(color_code color);
color_value get_b(color_code color);

