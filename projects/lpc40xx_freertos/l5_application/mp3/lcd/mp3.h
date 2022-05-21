#pragma once

#include "lcd_driver.h"
#include "song_list.h"

void mp3__init(void);
void mp3_controls_display(void);
void mp3__init_lcd_display(size_t song_index);
void song_playing(int song_number);
void setCursor(char col, char row);
void song_paused(int song_number);