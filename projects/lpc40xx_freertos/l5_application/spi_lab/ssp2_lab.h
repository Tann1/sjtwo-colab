#include <stdint.h>

#include "lpc40xx.h"



//MARCOS
#define SSP2_POWER (1U << 20); // Table.16 from LPC user manual
#define IOCON_FUNCTION_CLEAR (~0b111); //to clear the function in order to be at a deterministic value 
#define SSP2_IOCON_FUNCTION (0b100); //P1.0 SCK, P1.1 MOSI, P1.4 MISO

void ssp2_lab__init(uint32_t max_clock_mhz);

uint8_t ssp2_lab__exchange_byte(uint8_t data_out);