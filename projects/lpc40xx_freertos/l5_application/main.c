#include <stdio.h>

#include "FreeRTOS.h"
#include "clock.h"
#include "gpio.h"
#include "periodic_scheduler.h"
#include "semphr.h"
#include "task.h"

#include "uart_lab.h"

void uart_read_task(void *p) {
  char get_char;
  while (1) {
    // TODO: Use uart_lab__polled_get() function and printf the received value
    uart_lab__get_char_from_queue(&get_char, 500);
    // uart_lab__polled_get(UART_3, &get_char);
    fprintf(stderr, "%c\n", get_char);
    // vTaskDelay(500);
  }
}

void uart_write_task(void *p) {
  while (1) {
    // TODO: Use uart_lab__polled_put() function and send a value
    uart_lab__polled_put(UART_3, 'c');
    vTaskDelay(500);
  }
}

int main(void) {

  puts("Starting RTOS");

  uart_lab__init(UART_3, clock__get_peripheral_clock_hz(), 38400);
  uart__enable_receive_interrupt(UART_3);
  gpio__construct_with_function(GPIO__PORT_0, 0, GPIO__FUNCTION_2); // u3 txd
  gpio__construct_with_function(GPIO__PORT_0, 1, GPIO__FUNCTION_2); // u3 rxd

  xTaskCreate(uart_read_task, "uart read", 512, NULL, PRIORITY_LOW, NULL);
  xTaskCreate(uart_write_task, "uart write", 512, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
