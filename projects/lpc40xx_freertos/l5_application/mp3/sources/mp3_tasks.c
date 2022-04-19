#include "mp3_tasks.h"

void mp3_reader_task(void *p) {
  songname_s songname;

  while (1) {
    xQueueReceive(songname_q, &songname, portMAX_DELAY);
    fprintf(stderr, "song to play: %s\n", songname.name);
  }
}

