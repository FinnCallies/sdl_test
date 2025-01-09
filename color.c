#include "color.h"

color_code rgb2cc(color_value r, color_value g, color_value b)
{
        color_code ret = 0;

        ret |= r << 16;
        ret |= g << 8;
        ret |= b;

        return ret;
}

Color cc2Color(color_code color)
{
        Color ret;

        ret.r = (color >> 16) & 0xff;
        ret.g = (color >> 8) & 0xff;
        ret.b = color & 0xff;

        return ret;
}

color_code Color2cc(Color color)
{
        color_code ret = 0;

        ret |= color.r << 16;
        ret |= color.g << 8;
        ret |= color.b;

        return ret;
}

color_value get_r(color_code color)
{
        Color c = cc2Color(color);

        return c.r;
}

color_value get_g(color_code color)
{
        Color c = cc2Color(color);

        return c.g;
}

color_value get_b(color_code color)
{
        Color c = cc2Color(color);

        return c.b;
}

