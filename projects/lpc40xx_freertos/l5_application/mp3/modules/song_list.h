#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_SONG_FILE_SIZE 32

typedef struct {
  char filename[MAX_SONG_FILE_SIZE];
} mp3_item_s;

void sl__init(void);
void sl__refresh_list(void);
size_t sl__get_count(void);
mp3_item_s sl__get_item(size_t item_idx);