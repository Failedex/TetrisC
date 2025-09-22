#include <raylib.h>
#include "control.h"

KeyMap keymap_init(uint32_t key, double das, double arr, void (*func)())
{
    return (KeyMap) {
        .key = key,
        .start = 0,
        .das = das,
        .arr = arr,
        .auto_repeat = false,
        .holding = false,
        .func = func
    };
}

void parse_binds(KeyMap *maps, size_t size)
{
    for (size_t i = 0; i<size; i++) {
        if (IsKeyPressed(maps[i].key)) {
            maps[i].holding = true;
            maps[i].start = GetTime();
            (maps[i].func)();
        }
        if (IsKeyReleased(maps[i].key)) {
            maps[i].holding = false;
            maps[i].auto_repeat = false;
        }
        if (!maps[i].holding) {
            continue;
        }
        if (maps[i].auto_repeat) {
            if (maps[i].arr < 0.000001) {
                for (int j = 0; j<20; j++) {
                    (maps[i].func)();
                }
            } else {
                if (GetTime() - maps[i].start > maps[i].arr) {
                    maps[i].start = GetTime();
                    (maps[i].func)();
                }
            }
        } else {
            if (GetTime() - maps[i].start > maps[i].das) {
                maps[i].auto_repeat = true;
                maps[i].start = GetTime();
                (maps[i].func)();
            }
        }
    }
}
