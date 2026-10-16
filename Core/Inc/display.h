#ifndef DISPLAY_H
#define DISPLAY_H

#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>

void Display_Init(void);
void Display_WriteSpeed(uint16_t numCharsTyped);
void Display_ClearSpeed(void);

#endif
