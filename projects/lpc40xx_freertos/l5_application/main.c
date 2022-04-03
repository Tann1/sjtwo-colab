#include <math.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "clock.h"
#include "periodic_scheduler.h"
#include "queue.h"
#include "task.h"

#include "acceleration.h"
#include "ff.h"

#define SAMPLE_SIZE 100

QueueHandle_t sensor_queue;

float magnitude(acceleration__axis_data_s data) {
  uint32_t x = pow(data.x, 2);
  uint32_t y = pow(data.y, 2);
  uint32_t z = pow(data.z, 2);

  return pow(x + y + z, 0.5);
}

void producer(void *p) {
  uint16_t count = 0;
  float sum = 0;
  float avg = 0;
  while (1) {
    sum += magnitude(acceleration__get_data());
    count++;
    if (count == SAMPLE_SIZE) {
      avg = sum / SAMPLE_SIZE;
      xQueueSend(sensor_queue, &avg, 0);
      sum = 0;   // reset sum
      count = 0; // reset count
    }
    vTaskDelay(1);
  }
}

void consumer(void *p) {
  float data;
  float store_data[10]; // accumilate 10 samples before writing to the file 10 * 100ms = 1s
  const char *filename = "sensor.txt";
  FIL file; // file handle
  UINT bytes_written = 0;
  char string[64]; // format string to write into the file
  FRESULT result;
  while (1) {
    for (uint8_t sample_idx = 0; sample_idx < 10; ++sample_idx) { // accumlate the samples before writing to file
      if (xQueueReceive(sensor_queue, &data, portMAX_DELAY)) {
        // fprintf(stderr, "Received avg: %.2f\n", (double)data);
        store_data[sample_idx] = data;
      }
    } // end of for
    result = f_open(&file, filename, (FA_OPEN_APPEND | FA_WRITE));
    if (result == FR_OK) {
      for (uint8_t sample_idx = 0; sample_idx < 10; ++sample_idx) {
        sprintf(string, "%li %.2f\n", xTaskGetTickCount(), (double)store_data[sample_idx]);
        if (FR_OK == f_write(&file, string, strlen(string), &bytes_written)) {
          fprintf(stderr, "wrote: %s\n", string);
        } else {
          fprintf(stderr, "ERROR: Failed to write data to file\n");
        }
      }
      f_close(&file);
    } // end of if
    else {
      fprintf(stderr, "ERROR: Failed to open: %s result errno: %d\n", filename, result);
    }
  } // end of while
}

int main(void) {

  sensor_queue = xQueueCreate(1, sizeof(float));

  acceleration__init();

  xTaskCreate(producer, "producer", 512, NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(consumer, "consumer", 1024, NULL, PRIORITY_MEDIUM, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
