#include "ssp2_lab.h"

void ssp2_lab__init(uint32_t max_clock_mhz) {
  // Refer to LPC User Manual and set up the register bits correctly
  // a) Power on Peripheral
  // b) Setup control register CR0 and CR1
  // c) Setup prescalar register to be <= max_clock_mhz
}

uint8_t ssp2_lab__exchange_byte(uint8_t data_out) {
  // Configure the Data Register (DR) to send and receieve data by checking the SPI peripheral status register

  return 0;
}