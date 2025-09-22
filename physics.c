#include <stdlib.h>
#include <raylib.h>
#include "physics.h"
#include "randomizer.h"
#include "srs.h"

#define GRAVITY 0.5

static Position collision_data[7][4][4] = {
    {
        { {0, -1}, {0, 0}, {0, 1}, {0, 2} }, // I 
        { {-1, 0}, {0, 0}, {1, 0}, {2, 0} },
        { {0, -2}, {0, -1}, {0, 0}, {0, 1} },
        { {-2, 0}, {-1, 0}, {0, 0}, {1, 0} }, 
    },
    {
        { {0, -1}, {0, 0}, {0, 1}, {-1, -1} }, // J
        { {-1, 0}, {0, 0}, {1, 0}, {-1, 1} },  
        { {1, 1}, {0, -1}, {0, 0}, {0, 1} },   
        { {1, -1}, {-1, 0}, {0, 0}, {1, 0} },  
    },
    {
        { {0, -1}, {0, 0}, {0, 1}, {-1, 1} },  // L 
        { {-1, 0}, {0, 0}, {1, 0}, {1, 1} },   
        { {1, -1}, {0, -1}, {0, 0}, {0, 1} },  
        { {-1, -1}, {-1, 0}, {0, 0}, {1, 0} }, 
    },
    {
        { {0, 0}, {0, 1}, {-1, 0}, {-1, 1} }, // O
        { {0, 0}, {0, 1}, {1, 0}, {1, 1} }, 
        { {0, 0}, {0, -1}, {1, 0}, {1, -1} }, 
        { {0, 0}, {0, -1}, {-1, 0}, {-1, -1} }, 
    },
    {
        { {0, -1}, {0, 0}, {-1, 0}, {-1, 1} }, // S  
        { {-1, 0}, {0, 0}, {0, 1}, {1, 1} },    
        { {1, -1}, {1, 0}, {0, 0}, {0, 1} },    
        { {-1, -1}, {0, -1}, {0, 0}, {1, 0} },
    },
    {
        { {0, 0}, {0, 1}, {-1, 0}, {0, -1} }, // T
        { {0, 0}, {1, 0}, {0, 1}, {-1, 0} },  
        { {0, 0}, {0, -1}, {1, 0}, {0, 1} }, 
        { {0, 0}, {-1, 0}, {0, -1}, {1, 0} }, 
    },
    {
        { {-1, -1}, {-1, 0}, {0, 0}, {0, 1} }, // Z   
        { {-1, 1}, {0, 1}, {0, 0}, {1, 0} },    
        { {0, -1}, {0, 0}, {1, 0}, {1, 1} },    
        { {-1, 0}, {0, 0}, {0, -1}, {1, -1} },
    },
};

static Game game;
static bool *running = NULL;
static bool already_held = false;

static bool collision_detection(Position offset, uint8_t rotation) 
{
    for (size_t i = 0; i<4; i++) {
        Position check = collision_data[game.active.piece][(game.active.rotation + rotation) % 4][i];
        check.r += offset.r + game.active.center.r;
        check.c += offset.c + game.active.center.c;

        if (check.r >= game.height) {
            return true;
        }
        if (check.c < 0 || check.c >= game.width) {
            return true;
        }
        if ((game.board[check.r] & 1<<check.c) != 0) {
            return true;
        }
    }
    return false;
}

static void new_piece(Piece piece) 
{
    game.active = (ActivePiece) {
        .center = (Position) {
            .r = 0,
            .c = game.width/2 - 1 
        },
        .piece = piece,
        .rotation = 0,
        .spawntime = GetTime()
    };
    if (collision_detection((Position){0, 0}, 0)) {
        if (running != NULL && *running == true) {
            *running = false;
        }
    }
}

static void line_clear() 
{
    for (size_t i = 0; i<game.height; i++) {
        if (game.board[i] == (1<<game.width)-1) {
            for (size_t j = i; j>=1; j--) {
                game.board[j] = game.board[j-1];
            }
            game.board[0] = 0;
        }
    }
}

static void lock_piece() 
{
    for (size_t i = 0; i<4; i++) {
        Position pixel = collision_data[game.active.piece][game.active.rotation][i];
        pixel.r += game.active.center.r;
        pixel.c += game.active.center.c;
        game.board[pixel.r] |= (1 << pixel.c);
    }
    already_held = false;
    line_clear();
    new_piece(next_piece());
}

// If grouded and moving, reset the lock
static void lock_timer_reset() 
{
    if (collision_detection((Position){1, 0}, 0)) {
        game.active.spawntime = GetTime() - GRAVITY/4;
    }
}

void init_game(int width, int height, bool *remote) 
{
    game = (Game){
        .width = width,
        .height = height,
        .hold = N
    };
    game.board = calloc(sizeof(uint16_t), height);
    running = remote;
    init_bag();
    new_piece(next_piece());
}

void free_game() 
{
    free_bag();
    free(game.board);
}

void apply_gravity() 
{
    if (GetTime() - game.active.spawntime > GRAVITY) {
        game.active.spawntime = GetTime();
        if (collision_detection((Position){1, 0}, 0)) {
            lock_piece();
        } else {
            game.active.center.r += 1;
        }
    }
}

PixelState board_pixel(int r, int c) 
{
    if ((game.board[r] & (1 << c)) != 0) {
        return BOARD;
    } 

    for (size_t i = 0; i<4; i++) {
        Position pixel = collision_data[game.active.piece][game.active.rotation][i];
        pixel.r += game.active.center.r;
        pixel.c += game.active.center.c;
        if (r == pixel.r && c == pixel.c) {
            return ACTIVE;
        }

    }

    uint32_t offsetr = 0;
    while (!collision_detection((Position){offsetr, 0}, 0)) {
        offsetr += 1;
    }

    for (size_t i = 0; i<4; i++) {
        Position pixel = collision_data[game.active.piece][game.active.rotation][i];
        pixel.r += game.active.center.r + offsetr - 1;
        pixel.c += game.active.center.c;
        if (r == pixel.r && c == pixel.c) {
            return GHOST;
        }
    }

    return EMPTY;
};

PixelState hold_display(int r, int c)
{
    if (game.hold == N) {
        return GHOST;
    }
    for (size_t i = 0; i<4; i++) {
        Position pixel = collision_data[game.hold][0][i]; 
        pixel.r += 1;
        pixel.c += 1;
        if (r == pixel.r && c == pixel.c) {
            return ACTIVE;
        }
    }
    return GHOST;
}

PixelState next_display(int n, int r, int c)
{
    Piece p = peek_piece(n);
    for (size_t i = 0; i<4; i++) {
        Position pixel = collision_data[p][0][i]; 
        pixel.r += 2;
        pixel.c += 1;
        if (r == pixel.r && c == pixel.c) {
            return ACTIVE;
        }
    }
    return GHOST;
}

ActivePiece active_piece() 
{
    return game.active;
};

void move_left() 
{
    if (!collision_detection((Position){0, -1}, 0)) {
        game.active.center.c -= 1;
    }
    lock_timer_reset();
};

void move_right() 
{
    if (!collision_detection((Position){0, 1}, 0)) {
        game.active.center.c += 1;
    }
    lock_timer_reset();
};
void move_rotate(uint8_t rotation) {
    Position offsets[5]; 
    get_offsets(offsets, game.active.piece, game.active.rotation, (game.active.rotation + rotation) % 4);

    for (int i = 0; i<5; i++) {
        if (!collision_detection(offsets[i], rotation)) {
            game.active.rotation += rotation;
            game.active.rotation %= 4;
            game.active.center.r += offsets[i].r;
            game.active.center.c += offsets[i].c;
            lock_timer_reset();
            return;
        }
    }
};

void hard_drop() 
{
    while (!collision_detection((Position){1, 0}, 0)) {
        game.active.center.r += 1;
    }
    lock_piece();
};

void soft_drop() 
{
    game.active.spawntime = GetTime();
    if (!collision_detection((Position){1, 0}, 0)) {
        game.active.center.r += 1;
    }
}

void hold() 
{
    if (already_held) {
        return;
    }
    already_held = true;
    if (game.hold == N) {
        game.hold = game.active.piece;
        new_piece(next_piece());
    } else {
        Piece tmp = game.hold;
        game.hold = game.active.piece;
        new_piece(tmp);
    }
}
