#include "map.h"

Map new_map(int width, int height)
{
        Map ret = {.width = width, .height = height, .data = NULL};

        ret.data = (uint8_t **)calloc(height, sizeof(uint8_t *));
        for (int i = 0; i < height; i++)
                ret.data[i] = (uint8_t *)calloc(width, sizeof(uint8_t));

        return ret;
}

void init_map(Map *m, uint8_t v)
{
        for (int i = 0; i < (*m).height; i++)
                for (int j = 0; j < (*m).width; j++)
                        (*m).data[i][j] = v;
}

