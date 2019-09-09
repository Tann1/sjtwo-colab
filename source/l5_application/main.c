#include "lpc40xx.h"

#include "gpio.h"
#include "sys_time.h"

#include "FreeRTOS.h"
#include "task.h"

static void blink_task(void *params);

int main(void) {
  sys_time__init(configCPU_CLOCK_HZ);

  static gpio_s led1 = gpio__instantiate(gpio__port_1, 26);
  static gpio_s led2 = gpio__instantiate(gpio__port_2, 3);

  xTaskCreate((TaskFunction_t)blink_task, "task", 512U, &led1, PRIORITY_HIGH, NULL);
  xTaskCreate((TaskFunction_t)blink_task, "task", 512U, &led2, PRIORITY_HIGH, NULL);
  vTaskStartScheduler();

  /**
   * vTaskStartScheduler() should never return.
   *
   * Otherwise, it returning indicates there is not enough free memory or scheduler was explicitly terminated
   * CPU will now halt forever at this point.
   */
  while (1) {
  }

  return -1;
}

static void blink_task(void *params) {
  const gpio_s gpio = *(gpio_s *)params;

  gpio__set_as_output(gpio);
  while (1 == 1) {
    gpio__toggle(gpio);
    vTaskDelay(1000U);
  }
}