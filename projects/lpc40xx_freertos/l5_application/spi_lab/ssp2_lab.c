#include "ssp2_lab.h"

void init__ssp_cr0();
void init__ssp_cr1();
void init__ssp_prescalar(uint32_t);

void ssp2_lab__init(uint32_t max_clock_mhz) {
  // Refer to LPC User Manual and set up the register bits correctly
  // a) Power on Peripheral
  LPC_SC->PCONP |= SSP2_POWER;
  // b) Setup control register CR0 and CR1
  init__ssp_cr0();
  init__ssp_cr1();

  // c) Setup prescalar register to be <= max_clock_mhz
  init__ssp_prescalar(max_clock_mhz);
}

uint8_t ssp2_lab__exchange_byte(uint8_t data_out) {
  // Configure the Data Register (DR) to send and receieve data by checking the SPI peripheral status register

  LPC_SSP2->DR = data_out;

  do {
    SSP_STATUS.reg = LPC_SSP2->SR;
    // fprintf(stderr, "Busy exchaning. . .\n");
  } while (SSP_STATUS.BSY); // wait while SSP is transfering data still

  return (uint8_t)(LPC_SSP2->DR & 0xff);
}

void init__ssp_cr0() {
  SSP_CR0.reg = 0U;     // set to 0
  SSP_CR0.DSS = 0b0111; // 8 bits
  SSP_CR0.SCR = 0;      // take Serial Clock Rate out of the equation for simplicity

  LPC_SSP2->CR0 = SSP_CR0.reg;

  fprintf(stderr, "CR0: 0x%lx\n", LPC_SSP2->CR0);
}

void init__ssp_cr1() {
  SSP_CR1.reg = 0U;  // set to 0
  SSP_CR1.SSE = 0b1; // SSP enable

  LPC_SSP2->CR1 = SSP_CR1.reg;

  fprintf(stderr, "CR1: 0x%lx\n", LPC_SSP2->CR1);
}

void init__ssp_prescalar(uint32_t max_clock_mhz) {

  uint32_t scalar = clock__get_peripheral_clock_hz() / (max_clock_mhz * 1000 * 1000);

  LPC_SSP2->CPSR = scalar % 2 == 0 ? scalar : 0b100 << 0; // default to 4 := SCR = 96mhz / 4 = 24mhz
  fprintf(stderr, "CSPR: 0x%lx\n", LPC_SSP2->CPSR);
}