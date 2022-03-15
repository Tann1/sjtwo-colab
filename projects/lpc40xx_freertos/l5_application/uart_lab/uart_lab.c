#include "uart_lab.h"

LPC_UART_TypeDef *temp_uart;

void uart_lab__init(uart_number_e uart, uint32_t peripheral_clock, uint32_t baud_rate) {
  // Refer to LPC User manual and setup the register bits correctly
  // The first page of the UART chapter has good instructions
  // a) Power on Peripheral
  if (uart == UART_2) {
    LPC_SC->PCONP |= UART2_POWER;
  } else if (uart == UART_3)
    LPC_SC->PCONP |= UART3_POWER;

  // b) Setup DLL, DLM, FDR, LCR registers
  temp_uart = uart == UART_2 ? LPC_UART2 : LPC_UART3;
  temp_uart->LCR = 0x83; // Table.401 lpc manual enables DLAB[7] bit and sets read bit size
  temp_uart->FDR = 0x10; // Table.405 lpc manual divAddVal[3:0] = 0 and mulVal[7:4] = 1

  const uint16_t divider_16_bit = peripheral_clock / (16 * baud_rate);
  temp_uart->DLM = (divider_16_bit >> 8) & 0xFF; // Table.395
  temp_uart->DLL = (divider_16_bit >> 0) & 0xFF; // Table.396

  temp_uart->LCR = 0x03; // Disable DLAB[7] bit
}

// Read the byte from RBR and actually save it to the pointer
bool uart_lab__polled_get(uart_number_e uart, char *input_byte) {
  temp_uart = uart == UART_2 ? LPC_UART2 : LPC_UART3;
  // a) Check LSR for Receive Data Ready
  while (!(temp_uart->LSR & 0b1)) // meaning it is not ready yet Table.402
    ;
  // b) Copy data from RBR register to input_byte
  *input_byte = temp_uart->RBR;
  return true;
}

bool uart_lab__polled_put(uart_number_e uart, char output_byte) {
  temp_uart = uart == UART_2 ? LPC_UART2 : LPC_UART3;
  // a) Check LSR for Transmit Hold Register Empty
  while (!(temp_uart->LSR & (0b1 << 5))) // meaning not empty Table.402
    ;
  // b) Copy output_byte to THR register
  temp_uart->THR = output_byte;
  return true;
}