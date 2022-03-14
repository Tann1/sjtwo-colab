#include <stdio.h>

#include "FreeRTOS.h"
#include "gpio.h"
#include "periodic_scheduler.h"
#include "semphr.h"
#include "task.h"

#include "ssp2_lab.h"

gpio_s chip_select, mirror_cs;
SemaphoreHandle_t spi_bus_mutex;

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
  ssp2_lab__exchange_byte(0x9f);                        // send opcode
  data.manufacturer_id = ssp2_lab__exchange_byte(0xff); // get manufacturer id
  data.device_id_1 = ssp2_lab__exchange_byte(0xff);
  data.device_id_2 = ssp2_lab__exchange_byte(0xff);
  data.extended_device_id = ssp2_lab__exchange_byte(0xff);
  adesto_ds();

  return data;
}

void spi_task(void *p) {
  const uint32_t spi_clock_mhz = 24;

  ssp2_lab__init(spi_clock_mhz);

  configure_ssp2_functions();

  while (1) {
    adesto_flash_id_s _id = adesto_read_signature();
    fprintf(stderr, "manufacturer: 0x%X\n", _id.manufacturer_id);
    fprintf(stderr, "device id 1: 0x%X\n", _id.device_id_1);
    fprintf(stderr, "device id 2: 0x%X\n", _id.device_id_2);
    fprintf(stderr, "extended device id: 0x%X\n", _id.extended_device_id);
    vTaskDelay(500);
  }
}

// part 2
void spi_id_verification_task(void *p) {
  adesto_flash_id_s id = {0};
  adesto_ds(); // logical analyzer reason.
  while (1) {
    if (xSemaphoreTake(spi_bus_mutex, portMAX_DELAY)) { // guarded resource
      id = adesto_read_signature();
      xSemaphoreGive(spi_bus_mutex);
    }
    fprintf(stderr, "%s\n", (char *)(p));
    fprintf(stderr, "manufacturer: 0x%X\n", id.manufacturer_id);
    // When we read a manufacturer ID we do not expect, we will kill this task
    if (0x1F != id.manufacturer_id) {
      fprintf(stderr, "Manufacturer ID read failure\n");
      vTaskSuspend(NULL); // Kill this task
    }
    vTaskDelay(100);
  }
}

int main(void) {

  puts("Starting RTOS");
  // xTaskCreate(spi_task, "spi task 1", 512, 0, PRIORITY_LOW, 0); //part 1

  // part 2
  const uint32_t spi_clock_mhz = 1;

  ssp2_lab__init(spi_clock_mhz);
  configure_ssp2_functions();
  spi_bus_mutex = xSemaphoreCreateMutex();

  xTaskCreate(spi_id_verification_task, "spi task 1", 512, (void *)("task 1"), PRIORITY_HIGH, 0);
  xTaskCreate(spi_id_verification_task, "spi task 2", 512, (void *)("task 2"), PRIORITY_LOW, 0);

  // end of part 2
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

void configure_ssp2_functions(void) {
  gpio__construct_with_function(GPIO__PORT_1, 0, GPIO__FUNCTION_4); // SCK
  gpio__construct_with_function(GPIO__PORT_1, 1, GPIO__FUNCTION_4); // MOSI
  gpio__construct_with_function(GPIO__PORT_1, 4, GPIO__FUNCTION_4); // MISO
  chip_select = gpio__construct_as_output(GPIO__PORT_1, 10);        // CS
  mirror_cs = gpio__construct_as_output(GPIO__PORT_1, 31);          // mirror cs for logical analyzer
}

void adesto_cs(void) {
  gpio__reset(chip_select); // active low
  gpio__reset(mirror_cs);
}

void adesto_ds(void) {
  gpio__set(chip_select);
  gpio__set(mirror_cs);
}