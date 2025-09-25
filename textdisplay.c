#include "textdisplay.h"
#include <raylib.h>
#include <math.h>
#include <stdio.h>

static char* displaytext;
static char* subtext;
static char combotext[20]; 
static double timestamp = 0;

void display_text(Position textpos) 
{
    if (GetTime() - timestamp < 3.0) {
        double alpha = 255 - fmax(0.0, fabs(GetTime() - timestamp - 1.5) - 1.3)*255/0.2;
        DrawText(displaytext, textpos.c, textpos.r, 25, (Color){0, 0, 0, (int8_t)alpha});
        DrawText(subtext, textpos.c, textpos.r + 35, 20, (Color){0, 0, 0, (int8_t)alpha});
        DrawText(combotext, textpos.c, textpos.r + 60, 20, (Color){0, 0, 0, (int8_t)alpha});
    }
}

void set_text(char* text, char* stext)
{
    displaytext = text;
    subtext = stext;
    timestamp = GetTime();
}

void set_combo(uint32_t combo)
{
    if (combo > 1) {
        sprintf(combotext, "%d combo", combo - 1);
    } else {
        sprintf(combotext, "");
    }
}
