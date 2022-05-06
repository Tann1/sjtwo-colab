#include "song_list.h"
#include "ff.h"
#include "string.h"

#define MAX_SONGS 32

static mp3_item_s songs[MAX_SONGS];
static size_t song_size, idx;

void sl__init(void) {
  song_size = 0;
  idx = 0;
}

void sl__refresh_list(void) {
  FRESULT result;
  FILINFO file_info;
  const char *root_path = "/";

  sl__init(); // reset song_size and idx values
  DIR dir;
  result = f_opendir(&dir, root_path);
  if (result == FR_OK) {
    fprintf(stderr, "Opened dir successfully\n");
    while (song_size < MAX_SONGS) {
      result = f_readdir(&dir, &file_info);
      if (result != FR_OK || file_info.fname[0] == 0) {
        break;
      }
      if ((file_info.fattrib & AM_DIR) == 0) { // if its not a dir then its a file
        strncpy(songs[song_size++].filename, file_info.fname, MAX_SONG_FILE_SIZE);
        // fprintf(stderr, "%d. song name: %s\n", song_size, songs[song_size - 1].filename);
      }
    }
  } else {
    fprintf(stderr, "Failed to open root\n");
  }
}

size_t sl__get_count(void) { return song_size; }

mp3_item_s sl__get_item(size_t item_idx) { return songs[item_idx % song_size]; }
