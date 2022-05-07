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
  mp3__init(); // mp3.h
  while (1) {
    vTaskDelay(100);
  }
}

int main(void) {
  xTaskCreate(task_one, "mp3 init", 1024, NULL, PRIORITY_LOW, NULL);
  vTaskStartScheduler();
  return 0;
}
