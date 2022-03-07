
#include "adc_task.h"

static void pin_configure_adc_channel_as_io_pin(void);
void adc_task(void *p) {
  adc__initialize();
  adc__enable_burst_mode();
  fprintf(stderr, "control register: 0x%lx\n", LPC_ADC->CR); // making sure CR is configured properly
  pin_configure_adc_channel_as_io_pin();                     // Note that P1.31 is A type IOCON stands for Analog

  while (1) {
    const uint16_t adc_value = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_5);

    fprintf(stderr, "adc value: %u\n", adc_value);
    vTaskDelay(100);
  }
}

static void pin_configure_adc_channel_as_io_pin(void) {
  LPC_IOCON->P1_31 |= 0b011;        // ADC function
  LPC_IOCON->P1_31 &= ~(0b11 << 3); // set MODE to 0b00
  LPC_IOCON->P1_31 &= ~(1U << 7);   // set analog mode
}