#include <math.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "lcd_driver.h"
#include "mp3.h"
#include "queue.h"
#include "sj2_cli.h"
#include "task.h"

static void task_one(void *task_parameter) {
  size_t idx = 0;
  size_t song_size = sl__get_count();
  char buffer_msg[MAX_SONG_FILE_SIZE + 4]; // <song num, 2><., 1>< , 1><song name, 32>
  sl__refresh_list();

  while (1) {
    sprintf(buffer_msg, "%u. %s\n", (idx + 1) % song_size, sl__get_item(idx++).filename);
    lcd__send_row(buffer_msg);
    vTaskDelay(100);
  }
}

int main(void) {
  lcd__init(9600);

  xTaskCreate(task_one, "lcd transmitter", 1024, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler();
  return 0;
}
