#include "mp3.h"
#include <string.h>

extern const size_t COL;

void mp3__init_lcd_display(void) {
  size_t song_size = sl__get_count();
  size_t row_idx = 0;
  char row_buffer[COL]; // length of a given col in the lcd
  while (song_size > 0 && row_idx < 3 && row_idx < song_size) {
    memset(row_buffer, '\0', COL);
    sprintf(row_buffer, "%u. %s\n", (row_idx + 1) % song_size, sl__get_item(row_idx).filename);
    lcd__send_row(row_buffer, row_idx);
    row_idx++;
  }
  if (song_size <= 0) { // fill up space
    for (row_idx = 0; row_idx < 3; ++row_idx) {
      memset(row_buffer, ' ', COL);
      lcd__send_row(row_buffer, row_idx);
    }
  }
  sprintf(row_buffer, " ^ Up O sel !^ Down ");
  lcd__send_row(row_buffer, row_idx);
}

void mp3__init(void) {
  sl__refresh_list(); // init function for sl = song list
  lcd__init(9600);
  mp3__init_lcd_display();
}
