#include <raylib.h>
#include "physics.h"
#include "control.h"
#include "textdisplay.h"

#define boardWidth 10
#define boardHeight 20
#define pixel 30

#define DAS 0.133
#define ARR 0.010
#define SFD 0.010

int main(void)
{
    bool running;
    running = true;

    InitWindow((boardWidth + 12)*pixel, (boardHeight)*pixel, "tetris");
    SetTargetFPS(60);
    init_game(boardWidth, boardHeight, &running);

    KeyMap mapping[] = {
        keymap_init(KEY_LEFT, DAS, ARR, &move_left),
        keymap_init(KEY_RIGHT, DAS, ARR, &move_right),
        keymap_init(KEY_DOWN, 0, SFD, &soft_drop)
    };

    while (!WindowShouldClose() && running) 
    {
        parse_binds(mapping, 3);

        if (IsKeyPressed(KEY_Q)) {
            break;
        }
        if (IsKeyPressed(KEY_X)) {
            move_rotate(2);
        }
        if (IsKeyPressed(KEY_UP)) {
            move_rotate(1);
        }
        if (IsKeyPressed(KEY_Z)) {
            move_rotate(3);
        }
        if (IsKeyPressed(KEY_C)) {
            hold();
        }
        if (IsKeyPressed(KEY_SPACE)) {
            hard_drop();
        }

        apply_gravity();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // draw board
        for (int r = 0; r<boardHeight; r++) {
            for (int c = 0; c<boardWidth; c++) {
                Color color;
                switch (board_pixel(r, c)) {
                    case BOARD:
                        color = BLACK;
                        break;

                    case ACTIVE:
                        color = BLACK;
                        break;

                    case GHOST:
                        color = DARKGRAY;
                        break;

                    default:
                        color = LIGHTGRAY;
                        break;
                }
                DrawRectangle(c*pixel + 6*pixel, r*pixel, pixel, pixel, color);
            }
        }

        // draw hold and next pieces 

        for (int r = 0; r<4; r++) {
            for (int c = 0; c<4; c++) {
                if (hold_display(r, c) == ACTIVE) {
                    DrawRectangle(c*pixel + pixel, r*pixel + pixel, pixel, pixel, BLACK);
                }
            }
        }


        for (int i = 0; i<3; i++) {
            for (int r = 0; r<4; r++) {
                for (int c = 0; c<4; c++) {
                    if (next_display(i, r, c) == ACTIVE) {
                        DrawRectangle((c+7+boardWidth)*pixel, (r+1+4*i)*pixel, pixel, pixel, BLACK);
                    }
                }
            }
        }

        display_text((Position){6*pixel, pixel});

        EndDrawing();
    }

    free_game();
    CloseWindow();
    return 0;
}
