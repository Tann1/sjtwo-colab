#include "lcd_driver.h"
#include "clock.h"
#include <stdio.h>
#include <string.h>

void lcd__init(uint32_t baud_rate) {
  gpio__construct_with_function(GPIO__PORT_0, 0, GPIO__FUNCTION_2); // UART3 TX
  gpio__construct_with_function(GPIO__PORT_0, 1, GPIO__FUNCTION_2); // UART3 RX
  uart_lab__init(UART_3, clock__get_peripheral_clock_hz(), baud_rate);
}

void lcd__send_char(char output_byte) { uart_lab__polled_put(UART_3, output_byte); }

void lcd__send_row(char *message) {
  char row[21];
  char *walker = row;
  memset(row, '$', sizeof(row) - 1);
  row[20] = '\0';
  strncpy(row, message, 20);
  fprintf(stderr, "%s\n", row);
  while (*walker != '\0') {
    lcd__send_char(*walker);
    walker++;
  }
}