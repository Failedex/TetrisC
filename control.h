#ifndef CONTROL_H

#define CONTROL_H

#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint32_t key;
    double start;
    double das;
    double arr;
    bool auto_repeat;
    bool holding;
    void (*func)();
} KeyMap;

KeyMap keymap_init(uint32_t key, double das, double arr, void (*func)());

void parse_binds(KeyMap *maps, size_t size);

#endif // !CONTROL_H
