#include <math.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "clock.h"
#include "event_groups.h"
#include "lcd_driver.h"
#include "periodic_scheduler.h"
#include "queue.h"
#include "sj2_cli.h"
#include "task.h"

static void task_one(void *task_parameter) {
  while (1) {
    lcd__send_char('a');
    vTaskDelay(100);
  }
}

int main(void) {
  lcd__init(9600);

  xTaskCreate(task_one, "lcd transmitter", 512, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler();
  return 0;
}
