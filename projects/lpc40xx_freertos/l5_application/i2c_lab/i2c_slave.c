#include "i2c_slave.h"

void i2c2__slave_init(uint8_t slave_address_to_respond_to) {
  LPC_I2C2->ADR1 = slave_address_to_respond_to;
  LPC_I2C2->MASK1 = 0x00;
  LPC_I2C2->CONSET = (0b1 << 6 | 0b1 << 2); // I2EN and AA respectively
}

bool i2c_slave_callback__read_memory(uint8_t memory_index, uint8_t *memory) {
  if (memory_index >= MEM_SIZE)
    return false;
  *memory = slave_memory[memory_index];
  return true;
}

bool i2c_slave_callback__write_memory(uint8_t memory_index, uint8_t memory_value) {
  if (memory_index >= MEM_SIZE)
    return false;
  slave_memory[memory_index] = memory_value;
  return true;
}
