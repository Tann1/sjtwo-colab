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
void board_1_sender_task(void *p) {
  char number_as_string[16] = {0};

  while (true) {
    const int number = rand();
    sprintf(number_as_string, "%i", number);

    // Send one char at a time to the other board including terminating NULL char
    for (int i = 0; i <= strlen(number_as_string); i++) {
      uart_lab__polled_put(UART_3, number_as_string[i]);
      printf("Sent: %c\n", number_as_string[i]);
    }

    printf("Sent: %i over UART to the other board\n", number);
    vTaskDelay(3000);
  }
}

void board_2_receiver_task(void *p) {
  char number_as_string[16] = {0};
  int counter = 0;

  while (true) {
    char byte = 0;
    uart_lab__get_char_from_queue(&byte, portMAX_DELAY);
    printf("Received: %c\n", byte);

    // This is the last char, so print the number
    if ('\0' == byte) {
      number_as_string[counter] = '\0';
      counter = 0;
      printf("Received this number from the other board: %s\n", number_as_string);
    }
    // We have not yet received the NULL '\0' char, so buffer the data
    else {
      // TODO: Store data to number_as_string[] array one char at a time
      // Hint: Use counter as an index, and increment it as long as we do not reach max value of 16
      if (counter != 16)
        number_as_string[counter++] = byte;
    }
  }
}

int main(void) {

  puts("Starting RTOS");

  uart_lab__init(UART_3, clock__get_peripheral_clock_hz(), 38400);
  uart__enable_receive_interrupt(UART_3);
  gpio__construct_with_function(GPIO__PORT_0, 0, GPIO__FUNCTION_2); // u3 txd
  gpio__construct_with_function(GPIO__PORT_0, 1, GPIO__FUNCTION_2); // u3 rxd

  xTaskCreate(board_1_sender_task, "send data", 512, NULL, PRIORITY_LOW, NULL);
  xTaskCreate(board_2_receiver_task, "recieve data", 512, NULL, PRIORITY_LOW, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
