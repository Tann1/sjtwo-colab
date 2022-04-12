#pragma once
#include "lpc40xx.h"
#include <stdbool.h>

#define MEM_SIZE 256

volatile uint8_t slave_memory[MEM_SIZE];

void i2c2__slave_init(uint8_t slave_address_to_respond_to);
bool i2c_slave_callback__read_memory(uint8_t memory_index, uint8_t *memory);
bool i2c_slave_callback__write_memory(uint8_t memory_index, uint8_t memory_value);