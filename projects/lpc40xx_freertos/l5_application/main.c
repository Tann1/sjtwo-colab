#include <stdio.h>

#include "FreeRTOS.h"
#include "gpio.h"
#include "periodic_scheduler.h"
#include "task.h"

#include "ssp2_lab.h"

gpio_s chip_select;

void adesto_cs(void);
void adesto_ds(void);
void configure_ssp2_functions(void);

typedef struct {
  uint8_t manufacturer_id;
  uint8_t device_id_1;
  uint8_t device_id_2;
  uint8_t extended_device_id;
} adesto_flash_id_s;

adesto_flash_id_s adesto_read_signature(void) {
  adesto_flash_id_s data = {0};

  adesto_cs();
  data.manufacturer_id = ssp2_lab__exchange_byte(0x9F);
  adesto_ds();

  return data;
}

void spi_task(void *p) {
  const uint32_t spi_clock_mhz = 24;

  ssp2_lab__init(spi_clock_mhz);

  while (1) {
    adesto_flash_id_s id = adesto_read_signature();
    fprintf(stderr, "manufacturer: %u\n", id.manufacturer_id);
    vTaskDelay(500);
  }
}

int main(void) {

  puts("Starting RTOS");

  xTaskCreate(spi_task, "spi task", 512, 0, PRIORITY_LOW, 0);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void configure_ssp2_functions(void) {
  gpio__construct_with_function(GPIO__PORT_1, 0, GPIO__FUNCTION_4); // SCK
  gpio__construct_with_function(GPIO__PORT_1, 1, GPIO__FUNCTION_4); // MOSI
  gpio__construct_with_function(GPIO__PORT_1, 4, GPIO__FUNCTION_4); // MISO
  chip_select = gpio__construct_as_output(GPIO__PORT_1, 10);        // CS
}

void adesto_cs(void) {
  gpio__reset(chip_select); // active low
}

void adesto_ds(void) { gpio__set(chip_select); }