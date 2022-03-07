#include <stdio.h>

#include "FreeRTOS.h"
#include "periodic_scheduler.h"
#include "task.h"

#include "ssp2_lab.h"

int main(void) {

  puts("Starting RTOS");

  ssp2_lab__init(24);

  while (1)
    delay__ms(100);
  // vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
