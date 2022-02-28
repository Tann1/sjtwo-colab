#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "adc_task.h"
#include "periodic_scheduler.h"
#include "pwm_part.h"

int main(void) {

  xTaskCreate(adc_task, "sem", (512U * 4) / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
