#include <math.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "clock.h"
#include "event_groups.h"
#include "periodic_scheduler.h"
#include "queue.h"
#include "sj2_cli.h"
#include "task.h"


void simple_task(void *p) {
  while (1) {
    // fprintf(stderr, "hey you (:\n");
    vTaskDelay(100);
  }
}


int main(void) {


  sj2_cli__init();
  xTaskCreate(simple_task, "simple_task", 256, NULL, PRIORITY_LOW, &simple_handle);
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
