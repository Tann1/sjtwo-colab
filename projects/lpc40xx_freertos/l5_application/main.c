#include <math.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "mp3_tasks.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"
#include "struct_definition.h"
#include "task.h"

QueueHandle_t songname_q;
TaskHandle_t reader_handle;

int main(void) {
  sj2_cli__init();

  songname_q = xQueueCreate(1, sizeof(songname_s));
  xTaskCreate(mp3_reader_task, "mp3 reader", 1024, NULL, PRIORITY_MEDIUM, &reader_handle);
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
