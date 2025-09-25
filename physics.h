#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    I, 
    J,
    L,
    O,
    S,
    T,
    Z,
    N
} Piece;

typedef enum {
    EMPTY,
    BOARD, 
    ACTIVE,
    GHOST
} PixelState;

typedef struct {
    int32_t r;
    int32_t c;
} Position;

typedef struct {
    Piece piece;
    Position center;
    uint8_t rotation;
    double spawntime;
} ActivePiece;

typedef struct {
    int32_t width;
    int32_t height;
    uint16_t *board;
    Piece hold;
    bool already_held;
    ActivePiece active;
} Game;

void init_game(int width, int height, bool *remote);
void free_game();
void apply_gravity();

PixelState board_pixel(int r, int c);
PixelState hold_display(int r, int c);
PixelState next_display(int n, int r, int c);
ActivePiece active_piece();

void move_left();
void move_right();
void move_rotate(uint8_t rotate);
void hard_drop();
void soft_drop();
void hold();

#endif // !PHYSICS_H
