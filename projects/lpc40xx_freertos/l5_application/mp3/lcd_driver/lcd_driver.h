#pragma once

#include <stdint.h>

#include "gpio.h"
#include "uart_lab.h"

void lcd__init(uint32_t baud_rate);
void lcd__send_char(char output_byte);