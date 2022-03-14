#pragma once

#include <stdint.h>
#include <stdio.h>

#include "clock.h"
#include "lpc40xx.h"

#include "ssp2_register.h"

// MARCOS
#define SSP2_POWER (1U << 20); // Table.16 from LPC user manual

void ssp2_lab__init(uint32_t max_clock_mhz);

uint8_t ssp2_lab__exchange_byte(uint8_t data_out);