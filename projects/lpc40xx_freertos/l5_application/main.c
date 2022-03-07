#include <stdio.h>

#include "FreeRTOS.h"
#include "periodic_scheduler.h"
#include "task.h"

#include "ssp2_lab.h"

int main(void) {

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
