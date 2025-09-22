#include "srs.h"

static Position srstable[3][4][5] = {
    {
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        {{0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1}},
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        {{0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1}},
    },
    {
        {{0, 0}, {0, -1}, {0, 2}, {0, -1}, {0, 2}},
        {{0, -1}, {0, 0}, {0, 0}, {-1, 0}, {2, 0}},
        {{-1, -1}, {-1, 1}, {-1, -2}, {0, 1}, {0, -2}},
        {{-1, 0}, {-1, 0}, {-1, 0}, {1, 0}, {-2, 0}},
    },
    {
        {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
        {{1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}},
        {{1, -1}, {1, -1}, {1, -1}, {1, -1}, {1, -1}},
        {{0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}},
    },
};

void get_offsets(Position* offsets, Piece piece, uint8_t oldr, uint8_t newr)
{
    uint8_t table = 0;
    if (piece == I) {
        table = 1;
    } else if (piece == O) {
        table = 2;
    }

    for (int i = 0; i<5; i++) {
        Position oe = srstable[table][oldr][i];
        Position ne = srstable[table][newr][i];
        offsets[i].r = oe.r - ne.r;
        offsets[i].c = oe.c - ne.c;
    }
}
