#pragma once
#include <stdint.h>

union {
  uint32_t reg;
  struct {
    uint8_t DSS : 4;
    uint8_t FRF : 2;
    uint8_t CPOL : 1;
    uint8_t CPHA : 1;
    uint8_t SCR;
    uint16_t RESERVED;
  } __attribute__((packed));
} SSP_CR0;

union {
  uint32_t reg;
  struct {
    uint8_t LBM : 1;
    uint8_t SSE : 1;
    uint8_t MS : 1;
    uint8_t SOD : 1;
    uint32_t RESERVED : 28;
  } __attribute__((packed));
} SSP_CR1;

union {
  uint32_t reg;
  struct {
    uint8_t TFE : 1;
    uint8_t TNF : 1;
    uint8_t RNE : 1;
    uint8_t RFF : 1;
    uint8_t BSY : 1;
    uint32_t RESERVED : 27;
  } __attribute__((packed));

} SSP_STATUS;