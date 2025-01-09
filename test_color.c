#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "color.h"

void main()
{
        Color structcolor;
        color_code cc;

        cc = 0x010203;

        assert(get_r(cc) == 0x01);
        assert(get_g(cc) == 0x02);
        assert(get_b(cc) == 0x03);

        structcolor = cc2Color(cc);

        assert(structcolor.r == get_r(cc));
        assert(structcolor.g == get_g(cc));
        assert(structcolor.b == get_b(cc));

        assert(cc == Color2cc(structcolor));
}
