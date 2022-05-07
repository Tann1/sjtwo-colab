#pragma once

#include <stdint.h>

#include "gpio.h"
#include "uart_lab.h"

#define _ROW 4
#define _COL 4

void lcd__init(uint32_t baud_rate);
void lcd__send_char(char output_byte);
void lcd__send_row(char *message, size_t row_idx);